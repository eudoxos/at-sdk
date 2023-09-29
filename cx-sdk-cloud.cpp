#include<AT/cx/Image.h>
#include<AT/cx/c3d/Calib.h>
#include<AT/cx/c3d/PointCloud.h>
#include<AT/cx/CVUtils.h>
#include<filesystem>
#include<iostream>
#include<fstream>

int main(void){
	AT::cx::c3d::Calib calib_AT;
	calib_AT.load("at-c6-factory-calibration.xml");
	calib_AT.setParam(CX_3D_PARAM_METRIC_CACHE_MODE, int(1)); // some cache magic
	calib_AT.setParam(CX_3D_PARAM_METRIC_IDV,nanf);
	const char* bufFile="range-buffer-50x2048-C16.bin";
	size_t coorSize=std::filesystem::file_size(bufFile);
	char* coorBuf=new char[coorSize];
	std::ifstream(bufFile,std::ios::binary).read(coorBuf,coorSize);
	std::cerr<<bufFile<<": "<<coorSize<<" b read"<<std::endl;
	AT::cx::Image rangeImg(50,2048,cx_pixel_format::CX_PF_COORD3D_C16,coorBuf,coorSize);
	AT::cx::c3d::PointCloud pc(rangeImg.height(),rangeImg.width());
	AT::cx::c3d::calculatePointCloud(calib_AT,rangeImg,pc);
	assert(pc.points.pixelFormat()==cx_pixel_format::CX_PF_COORD3D_ABC32f);
	pc.save("cloud.xyz");
	pc.save("cloud.ply");
}

