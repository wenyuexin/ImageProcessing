模板匹配函数：

1 tempMatch

2 matching

int matching(const Mat &image, const Mat &templ, double CompressionRatio, Point &matchLoc) 

功能：基于二次匹配方法的模板匹配函数

输入：image 待匹配的图像

     templ 用于匹配的模板
     
     CompressionRatio 预匹配的图像压缩率
     
输出：matchLoc 匹配坐标

基于opencv3中的matchTemplate函数与图像金字塔实现的模板匹配，并返回模板templ在待匹配图像image中的坐标。

该功能包含预匹配与二次匹配两个步骤：
预匹配中，先以CompressionRatio的图像压缩率对模板templ和待匹配图像image进行压缩，获取匹配坐标matchLoc1。

然后根据压缩率对坐标matchLoc1进行转换，得到原始模板templ在原始待匹配图像image中的坐标matchLoc2。

因此可以通过压缩率与匹配坐标matchLoc2得到限定范围的待匹配图像image2，最终能够通过原始模板在image2中的匹配坐标

得到原始模板templ在原始待匹配图像image中的准确位置。

此方法的优势在于，在不影响匹配结果的情况下，对大尺寸图像做模板匹配是具有更快的匹配速度

值得注意的是，matching函数在调用opencv的matchTemplate函数使用TM_CCOEFF_NORMED方法（归一化的互相关匹配方法），

如果想使用其他匹配方法，可以使用自定义的tempMatch函数进行修改

