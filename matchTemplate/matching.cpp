using cv::Mat;
 using cv::Point;
 using cv::Rect;
 
 /**
 * 功能：基于二次匹配方法的模板匹配函数
 * 输入：image 待匹配的图像
 *       templ 用于匹配的模板
 *       CompressionRatio 预匹配的图像压缩率
 * 输出：matchLoc 匹配坐标
 */
int matching(const Mat &image, const Mat &templ, double CompressionRatio, Point &matchLoc) 
{
	int imageH = image.rows; //待匹配图像的高度
	int imageW = image.cols; //待匹配图像的宽度
	int templH = templ.rows; //匹配模板的高度
	int templW = templ.cols; //匹配模板的宽度

	//模板大小不能大于样本大小
	if (imageH < templH && imageW < templW)
		return -1;

	//匹配相关的临时变量
	double minValue, maxValue;
	Mat imgResult, imgROI;
	Rect roiRect;
	Point minLoc, maxLoc;

	/*预匹配：初步确定匹配坐标*/
	if (0<CompressionRatio && CompressionRatio<1)
	{
		//图像压缩
		Mat imageResize_M1, templResize_M1;
		cv::resize(image, imageResize_M1, Size(int(imageW*CompressionRatio),int(imageH*CompressionRatio)));
		cv::resize(templ, templResize_M1, Size(int(templW*CompressionRatio),int(templH*CompressionRatio)));

		//预匹配
		cv::matchTemplate(imageResize_M1, templResize_M1, imgResult, TM_CCOEFF_NORMED); //预匹配
		cv::minMaxLoc(imgResult, &minValue, &maxValue, &minLoc, &maxLoc, Mat()); //获取匹配点

		//预匹配坐标转换为未压缩前的坐标
		int roiCentorY = cvRound(double(maxLoc.y) / CompressionRatio); //坐标转换
		int roiCentorX = cvRound(double(maxLoc.x) / CompressionRatio);
		if (roiCentorY > imageH - templH) roiCentorY = imageH - templH; //防止越界
		if (roiCentorX > imageW - templW) roiCentorX = imageW - templW;

		//截取二次匹配的小区域
		int roiSideLen = int(ceil(2.2 / CompressionRatio)); //ROI区域的边长
		roiSideLen = (roiSideLen%2 != 0) ? roiSideLen+1 : roiSideLen; //取偶数
		roiSideLen = (roiSideLen >= 2) ? roiSideLen : 2; //防止ROI区域太小

		Rect rect_image = { Point(0,0), image.size() };
		Point roiLoc = Point(roiCentorX, roiCentorY);
		roiLoc -= Point(int(0.5*roiSideLen), int(0.5*roiSideLen));
		Size roiSize = templ.size() + Size(roiSideLen, roiSideLen);
		roiRect = Rect(roiLoc, roiSize) & rect_image;
		imgROI = image(roiRect); //截取样本ROI区域
	}
	else
	{
		imgROI = image; //不压缩
	}
	
	/*二次匹配*/
	cv::matchTemplate(imgROI, templ, imgResult, TM_CCOEFF_NORMED); //图像匹配
	cv::minMaxLoc(imgResult, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
	matchLoc = maxLoc + roiRect.tl(); //匹配坐标
	return 0;
}
