#include "MagicBlackCatFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicBlackCatFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cangwang 2018.12.1
 */
MagicBlackCatFilter::MagicBlackCatFilter(){

}

MagicBlackCatFilter::MagicBlackCatFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"blackcat.glsl")),mToneCurveTexture(0){

}

MagicBlackCatFilter::~MagicBlackCatFilter() {

}

void MagicBlackCatFilter::onDestroy() {
    glDeleteTextures(1,&mToneCurveTexture);
    mToneCurveTexture=0;
}

void MagicBlackCatFilter::onDrawArraysPre() {
    if(this->mToneCurveTexture !=0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glUniform1i(this->mToneCurveTextureUniformLocation,3);
    }
}

void MagicBlackCatFilter::onDrawArraysAfter() {
    if (this->mToneCurveTexture != 0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicBlackCatFilter::onInit() {
    GPUImageFilter::onInit();
    mToneCurveTextureUniformLocation = glGetUniformLocation(mGLProgId,"curve");
}

void MagicBlackCatFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    glGenTextures(1,&mToneCurveTexture);
    glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //c++中没有byte类型，用unsigned char代替
    unsigned char* arrayOfByte = (unsigned char*)malloc(sizeof(char)*2048);
    unsigned char arrayOfInt1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 7, 8, 10, 11, 13, 15, 16, 18, 20, 21, 23, 24, 26, 28, 29, 31, 33, 34, 36, 37, 39, 41, 42, 44, 45, 47, 49, 50, 52, 53, 55, 57, 58, 60, 61, 63, 65, 66, 68, 69, 71, 72, 74, 76, 77, 79, 80, 82, 83, 85, 86, 88, 90, 91, 93, 94, 96, 97, 99, 100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 127, 128, 129, 131, 132, 134, 135, 136, 138, 139, 141, 142, 143, 145, 146, 147, 149, 150, 151, 153, 154, 155, 157, 158, 159, 160, 162, 163, 164, 165, 167, 168, 169, 170, 172, 173, 174, 175, 176, 178, 179, 180, 181, 182, 183, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 199, 200, 201, 202, 203, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 224, 225, 226, 227, 227, 228, 229, 229, 230, 231, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 238, 239, 240, 240, 241, 242, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 248, 249, 250, 250, 251, 251, 252, 253, 253, 254, 254, 255 };
    unsigned char arrayOfInt2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 7, 8, 10, 11, 13, 15, 16, 16, 18, 20, 21, 23, 24, 26, 28, 29, 31, 33, 34, 36, 37, 39, 41, 41, 42, 44, 45, 47, 49, 50, 52, 53, 55, 57, 58, 60, 61, 63, 65, 66, 68, 69, 71, 72, 74, 76, 77, 79, 80, 82, 83, 85, 86, 86, 88, 90, 91, 93, 94, 96, 97, 99, 100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 127, 128, 129, 131, 134, 135, 136, 138, 139, 141, 142, 143, 145, 146, 147, 149, 150, 151, 153, 154, 155, 157, 158, 159, 160, 162, 163, 164, 165, 167, 168, 169, 170, 172, 173, 174, 175, 176, 179, 180, 181, 182, 183, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 199, 200, 201, 203, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 215, 216, 217, 219, 220, 220, 221, 222, 223, 224, 224, 225, 226, 227, 227, 228, 229, 229, 230, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 238, 239, 240, 240, 242, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 255 };
    unsigned char arrayOfInt3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3, 5, 7, 8, 10, 10, 11, 13, 15, 16, 18, 20, 20, 21, 23, 24, 26, 28, 29, 29, 31, 33, 34, 36, 37, 39, 39, 41, 42, 44, 45, 47, 49, 50, 50, 52, 53, 55, 57, 58, 60, 61, 63, 63, 65, 66, 68, 69, 71, 72, 74, 76, 77, 79, 79, 80, 82, 83, 85, 86, 88, 90, 91, 93, 94, 96, 97, 99, 100, 100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 127, 128, 129, 131, 132, 134, 135, 136, 138, 139, 141, 142, 143, 145, 146, 147, 149, 150, 151, 153, 154, 155, 157, 158, 159, 160, 162, 164, 165, 167, 168, 169, 170, 172, 173, 174, 175, 176, 178, 179, 180, 181, 182, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 199, 200, 201, 202, 203, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 215, 216, 218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 227, 228, 229, 229, 230, 232, 232, 233, 234, 234, 235, 236, 236, 238, 238, 239, 240, 240, 241, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 249, 250, 250, 251, 251, 252, 253, 254, 254, 255 };
    for (int i = 0; i < 256; i++){
        arrayOfByte[i*4]= arrayOfInt1[i];
        arrayOfByte[(1+i*4)]= arrayOfInt2[i];
        arrayOfByte[(2+i*4)]= arrayOfInt3[i];
        arrayOfByte[(3+i*4)] = static_cast<unsigned char>(-1);
    }
    unsigned char arrayOfInt4[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 12, 13, 13, 14, 14, 15, 16, 17, 18, 18, 19, 19, 20, 21, 21, 23, 24, 25, 26, 26, 26, 27, 28, 29, 30, 31, 32, 32, 33, 34, 35, 36, 37, 38, 38, 38, 39, 40, 41, 43, 44, 45, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 57, 58, 59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 70, 71, 72, 74, 75, 76, 76, 77, 78, 79, 80, 81, 82, 82, 83, 85, 86, 87, 88, 89, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 101, 102, 103, 104, 105, 107, 107, 108, 109, 111, 112, 113, 114, 114, 115, 116, 117, 119, 120, 120, 121, 122, 123, 124, 125, 126, 126, 127, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 139, 141, 142, 143, 144, 145, 145, 146, 147, 148, 149, 151, 151, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 163, 165, 166, 167, 168, 169, 170, 170, 171, 172, 173, 174, 175, 177, 178, 179, 180, 181, 182, 183, 183, 184, 185, 186, 189, 189, 190, 191, 192, 193, 194, 195, 195, 196, 198, 199, 201, 202, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 213, 214, 214, 215, 216, 218, 219, 220, 221, 221, 222, 223, 225, 227, 227, 228, 229, 230, 230, 230, 230, 230, 230, 230, 230, 230 };
    unsigned char arrayOfInt5[] = { 0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 8, 8, 9, 10, 11, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 39, 40, 41, 42, 43, 44, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 76, 77, 78, 79, 81, 82, 83, 84, 86, 87, 88, 89, 91, 92, 93, 95, 96, 97, 99, 100, 101, 103, 104, 105, 107, 108, 109, 111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 126, 128, 129, 131, 132, 134, 135, 137, 138, 140, 141, 142, 144, 145, 147, 148, 149, 151, 152, 154, 155, 156, 158, 159, 160, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173, 175, 176, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 225, 225, 226, 227, 228, 229, 229, 230, 231, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 238, 239, 240, 240, 241, 241, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 252, 253, 253, 254, 254, 254, 255, 255 };
    unsigned char arrayOfInt6[] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 29, 31, 31, 31, 31, 32, 32, 33, 33, 34, 34, 34, 34, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 47, 47, 48, 48, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 103, 104, 105, 107, 108, 110, 111, 113, 115, 116, 118, 119, 120, 122, 123, 125, 127, 128, 130, 132, 134, 135, 137, 139, 141, 143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 163, 165, 167, 169, 171, 173, 175, 178, 180, 182, 185, 187, 189, 192, 194, 197, 199, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 236, 239, 241, 245, 247, 250, 252, 255 };
    for (int j = 0; j < 256; ++j) {
        arrayOfByte[(1024 + j * 4)] = arrayOfInt4[j];
        arrayOfByte[(1 + (1024 + j * 4))] = arrayOfInt5[j];
        arrayOfByte[(2 + (1024 + j * 4))] = arrayOfInt6[j];
        arrayOfByte[(3 + (1024 + j * 4))] = static_cast<unsigned char>(-1);
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,2,0,GL_RGBA,GL_UNSIGNED_BYTE,arrayOfByte);
}