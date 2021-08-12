#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <dirent.h>

using namespace std;
// https://pytorch.org/tutorials/advanced/cpp_export.html

vector<torch::jit::script::Module> modules;

extern "C" {
#include "setu.h"

int load_module(const char* path) {
    modules.push_back(torch::jit::load(path));
    return modules.size() - 1;
}

int predict_file(const char* path, int index) {
    if (index < 0 || index >= modules.size()) return -2;
    auto image = cv::imread(path, cv::ImreadModes::IMREAD_COLOR);
    if (image.size().empty()) return -1;
    cv::Mat image_transformed;
    cv::resize(image, image_transformed, cv::Size(224, 224));
    cv::cvtColor(image_transformed, image_transformed, cv::COLOR_BGR2RGB);
    //图像转换为tensor
    torch::Tensor image_tensor = torch::from_blob(image_transformed.data,
                                                {image_transformed.rows, image_transformed.cols, 3},
                                                torch::kByte);
    image_tensor = image_tensor.permute({2, 0, 1});
    image_tensor = image_tensor.toType(torch::kFloat);
    image_tensor = image_tensor.div(255);
    image_tensor = image_tensor.unsqueeze(0);
    //前向传播
    at::Tensor output = modules[index].forward({image_tensor}).toTensor();
    return get<1>(output.max(1, true)).item<int>();
}
}

#ifdef DEBUG

static void getfiles(string path, vector<string>& files) {
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(path.c_str());
    while ((ptr = readdir(dir)) != NULL) {
        //跳过'.'和'..'两个目录
        if(ptr->d_name[0] == '.') continue;
        files.push_back(ptr->d_name);
    }
}

vector<string> predict_folder(const char * path) {
    vector<string> files, predicts;
    // 获取该路径下的所有文件
    getfiles(path, files);
    int size = files.size();
    int last = strlen(path);
    char* buf = (char*)malloc(last + MAX_FILENAME_LEN);
    memcpy(buf, path, last);
    for (int i = 0; i < size; i++) {
        ostringstream ostr;
        auto filename = files[i].c_str();
        int len = strlen(filename);
        if (len < MAX_FILENAME_LEN - 1) {
            memcpy(buf+last, filename, len);
            buf[last+len] = 0;
            ostr << "<" << predict_file(buf, 0) << ">: " << filename << endl;
            predicts.push_back(ostr.str());
        }
    }
    free(buf);
    return predicts;
}

#define MODULE_PATH argv[1]
#define IMAGE_PATH argv[2]
int main(int argc, const char* argv[]) {
    auto ts = time(NULL);
    load_module(MODULE_PATH);
    cout << "Load module cost: " << time(NULL) - ts << "s." << endl;
    ts = time(NULL);
    auto p = predict_folder(IMAGE_PATH);
    cout << "Predict " << p.size() << " files cost: " << time(NULL) - ts << "s." << endl;
    for (int i = 0; i < p.size(); i++) {
        cout << p[i];
    }
    return 0;
}

#endif