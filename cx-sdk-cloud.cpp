#include<AT/cx/Image.h>
#include<AT/cx/c3d/Calib.h>
#include<AT/cx/c3d/PointCloud.h>
#include<AT/cx/CVUtils.h>
#include<filesystem>
#include<iostream>
#include<fstream>

int main(void){
	/*
	Calibration XML as stored in the sensor. No need to adjust further:
	- buffer was recorder with Scan3dCoordinateScale=0.03125, same as factory calibration
	- not binning/decimation being used
	*/
	AT::cx::c3d::Calib calib_AT;
	calib_AT.load("at-c6-factory-calibration.xml");
	calib_AT.setParam(CX_3D_PARAM_METRIC_IDV,nanf);
	/* load relevant buffer part (range data) from file. The format is COORD3D_C16. */
	const char* bufFile="range-buffer-50x2048-C16.bin";
	size_t coorSize=std::filesystem::file_size(bufFile);
	char* coorBuf=new char[coorSize];
	std::ifstream(bufFile,std::ios::binary).read(coorBuf,coorSize);
	std::cerr<<bufFile<<": "<<coorSize<<"b read"<<std::endl;
	/* create new image referencing our buffer */
	AT::cx::Image rangeImg(50,2048,cx_pixel_format::CX_PF_COORD3D_C16,coorBuf,coorSize);
	/* compute point cloud */
	AT::cx::c3d::PointCloud pc(rangeImg.height(),rangeImg.width());
	AT::cx::c3d::calculatePointCloud(calib_AT,rangeImg,pc);
	assert(pc.points.pixelFormat()==cx_pixel_format::CX_PF_COORD3D_ABC32f);
	/* traverse the cloud, counting non-0,0,0 points */
	int nonzero=0;
	for(size_t y=0; y<pc.points.height(); y++){
		for(size_t x=0; x<pc.points.width(); x++){
			auto p=pc.points.at<AT::cx::Point3f>(y,x);
			if(p.x!=0 || p.y!=0 || p.z!=0) nonzero++;
		}
	}
	if(nonzero==0){
		std::cerr<<"Error: no valid (non-zero) points in the cloud."<<std::endl;
		exit(1);
	} else {
		/* save the point cloud to files for inspection */
		pc.save("cloud.xyz");
		pc.save("cloud.ply");
	}
}

