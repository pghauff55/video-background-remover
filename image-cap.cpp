/*! \mainpage

* 
* This code is a C++ program that uses the OpenCV library to perform video camera processing. 
* It begins by creating a VideoCapture object with the parameter 0 that indicates that the 
* camera device number 0 should be used. It then checks to see if the camera was opened 
* successfully, and if not, exits the program. It then gets the width and height of the 
* frame from the video capture and stores them in dWidth and dHeight. After that, it 
* creates a window named MyVideo. 

* It then reads the first frame from the camera into frame_still and checks again to see 
* if the read was successful. If not, it breaks out of the loop. It then sets the size 
* of the template that will be used when performing template matching and gets the
*  number of columns and rows for the image by dividing the frame width and height by 
* the size of the template respectively. 


*It then has a while loop which reads a new frame from the camera and stores it in frame.
*  It then applies a Gaussian blur to the frame and performs template matching to find 
* the best matches for the given template. If a match is found, it is highlighted in the 
* frame. It then checks each pixel in the frame against the pixel in the original 
* frame_still to find the delta color for each pixel. If the delta color is smaller 
* than a given threshold, it will change the pixel color to black. It then displays 
* the frame in the MyVideo window and waits for a key press. 
* If the key is Esc, it will break out of the loop. If the key is space, it will read
*  a new frame into frame_still, apply the same processing as before and store the 
* results into a vector. After that, the loop will start again. Finally, it returns 
* 0 when the loop has completed. 

* \section intro_sec Introduction
* # Introduction
* This code is written in C++ and uses OpenCV to process a Mat frame_still. 
* The code is used to "map" the image by dividing it into smaller pieces,
*  by setting the crop width (crop_width) and crop height (crop_height) 
* to the given template size and finding the number of pieces N and M in 
* the image. The code then starts a loop from 0 to M (rows) and inside this 
* loop, another loop from 0 to N (columns) is created. The offset_x and
* offset_y values are used to define a rectangular region of interest 
* (ROI) which is used to crop the image into pieces. 
* The cropped pieces are then pushed into a vector (result) and the 
* total number of pieces is returned. The code also checks whether any 
* features are detected using feature detection and skips any pieces with
*  no features.

*/
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;


#define THRESHOLD 20
float factor_tile_size=0.3;
#define TILESIZE 20

class BackGroundRemover{
Mat *Image;
vector<Mat *> list_Image;
VideoCapture *cap;
int dWidt h,dHeight;
vector<Mat *> list_tile_features;
	
public:
    BackGroundRemover(int WIDTH,int HEIGHT){
		Mat frame;
		/** Open video capture device 0. */
		this->cap=new VideoCapture(0);
		this->cap->set(CAP_PROP_FRAME_WIDTH, WIDTH);
		this->cap->set(CAP_PROP_FRAME_HEIGHT,HEIGHT);
		if (!cap->isOpened())  /** if not success, exit program*/
		{
        cout << "Cannot open the video cam" << endl;
        return;
		}

		this->dWidth = WIDTH; /**get the width of frames of the video*/
		this->dHeight = HEIGHT; /**get the height of frames of the video*/
		
		bool bSuccess = cap->read(frame); /** read a new frame from video*/
	
		if (!bSuccess) /**if not success, break loop*/
		{
			cout << "Cannot read a frame from video stream" << endl;
			return;
		}
		this->Image=new Mat(frame);
		//GaussianBlur(*(this->Image),*(this->Image), Size(5, 5), 0);
		//ColoriseBackground();
		list_Image.push_back(this->Image);
		//Create_Tiles(TILESIZE);
		//Save_Tiles(TILESIZE);
	}
	~BackGroundRemover(){
		
		delete this->cap;
		delete this->Image;
		for(int i=0;i<list_tile_features.size();i++){
			
			delete list_tile_features[i];
		}
		for(int i=0;i<list_Image.size();i++){
			
			delete list_Image[0];
		}
	}
	
	
	void UpdateBackground(){
		Mat frame;
		bool bSuccess = cap->read(frame); /// read a new frame from video
			cout << "reading still frame" << endl;
			if (!bSuccess) ///if not success, break loop
			{
				cout << "Cannot read a frame from video stream" << endl;
				return;
			}
		
		this->Image=new Mat(frame);
		//GaussianBlur(*(this->Image),*(this->Image), Size(5, 5), 0);
		//ColoriseBackground();
		list_Image.push_back(this->Image);
		
	}
	
	
	void ColoriseBackground(){
	unsigned char *pImgData = (unsigned char *)this->Image->data;
	cout<<"loop:"<<this->Image->rows<<","<<this->Image->cols<<","<<this->Image->channels()<<endl;
	// loop through rows, columns and channels
	for (int row = 0; row < this->Image->rows; row++){
		for (int column = 0; column < this->Image->cols; column++){
			float value0,value1,value2;
			int value_out0,value_out1,value_out2;
				int index=this->Image->channels() * (this->Image->cols * row + column);
				value0 = pImgData[index];
				value1 = pImgData[index+1];
				value2 = pImgData[index+2];
				
				//cout<<value0<<","<<value1<<","<<value2<<endl;
				Colorise(value0,value1,value2,&value_out0,&value_out1,&value_out2);
				//cout<<value_out0<<","<<value_out1<<","<<value_out2<<endl;
				pImgData[this->Image->channels() * (this->Image->cols * row + column) ]=(unsigned char)value_out0;
				pImgData[this->Image->channels() * (this->Image->cols * row + column)+1]=(unsigned char)value_out1;
				pImgData[this->Image->channels() * (this->Image->cols * row + column)+2]=(unsigned char)value_out2;

				//cout<<row<<","<<column<<endl;
				
			}
		}
	}
	
	
	void Show(){
		Mat frame;
		bool bSuccess = cap->read(frame); /** read a new frame from video*/
        if (!bSuccess) /**if not success, break loop*/
        {
             cout << "Cannot read a frame from video stream" << endl;
             return;
        }
        //GaussianBlur(frame,frame, Size(5, 5), 0);
        unsigned char *pImgData = (unsigned char *)frame.data;
		//cout<<"loop:"<<this->Image->rows<<","<<this->Image->cols<<","<<this->Image->channels()<<endl;
		
		// loop through rows, columns and channels
		for (int row = 0; row < frame.rows; row++){
			for (int column = 0; column < frame.cols; column++){
				float value0,value1,value2;
				int value_out0,value_out1,value_out2;
					int index=frame.channels() * (frame.cols * row + column);
					value0 = pImgData[index];
					value1 = pImgData[index+1];
					value2 = pImgData[index+2];
					
					//cout<<value0<<","<<value1<<","<<value2<<endl;
					Colorise2(value0,value1,value2,&value_out0,&value_out1,&value_out2);
					//cout<<value_out0<<","<<value_out1<<","<<value_out2<<endl;
					pImgData[frame.channels() * (frame.cols * row + column) ]=(unsigned char)value_out0;
					pImgData[frame.channels() * (frame.cols * row + column)+1]=(unsigned char)value_out1;
					pImgData[frame.channels() * (frame.cols * row + column)+2]=(unsigned char)value_out2;

					//cout<<row<<","<<column<<endl;
					
				}
			}
        
        imshow("MyVideo2", frame); ///show the frame in "MyVideo" window
        
	}
	
	
	void Process(int tile_size){
		Mat frame;
		bool bSuccess = cap->read(frame); /** read a new frame from video*/
        if (!bSuccess) /**if not success, break loop*/
        {
             cout << "Cannot read a frame from video stream" << endl;
             return;
        }
        //GaussianBlur(frame,frame, Size(5, 5), 0);
        int tile_area=tile_size*tile_size;
		int count_threshold=(1.0-factor_tile_size)*tile_area;

		int M=dWidth/tile_size;
		int N=dHeight/tile_size;
		//cout<<"N:"<<N<<"M:"<<M<<endl;			
		int *tile_matrix=new int[N*M];
		for(int i=0;i<N*M;i++)tile_matrix[i]=0;
		
		for(int i=0;i<list_Image.size();i++){
			this->Image=list_Image[i];
			unsigned char *pImgData0 = (unsigned char *)this->Image->data;
			unsigned char *pImgData1 = (unsigned char *)frame.data;
			
		
			cout<<"i:"<<i<<endl;
			for (int y = 0; y < N; y++){
			for (int x = 0; x < M; x++){
					int count_tile_equals=0;
					if(tile_matrix[x*N+y]==0){
					for (int row = y*tile_size; row < (y+1)*tile_size; row++){
						int count_column_equals=0;
						for (int column = x*tile_size; column < (x+1)*tile_size; column++){
							
								int value0,value1,value2,value3,value4,value5;
								int value_out0,value_out1,value_out2;
								int delta_value0,delta_value1,delta_value2;
								value0 = pImgData1[frame.channels() * (frame.cols * row + column) ];
								value1 = pImgData1[frame.channels() * (frame.cols * row + column)+1];
								value2 = pImgData1[frame.channels() * (frame.cols * row + column)+2];
								//cout<<"row:"<<row<<"column:"<<column<<endl;
								//cout<<"Input Image:"<<value0<<","<<value1<<","<<value2<<endl;
								//Colorise(value0,value1,value2,&value_out0,&value_out1,&value_out2);
								value_out0=value0;
								value_out1=value1;
								value_out2=value2;
								
								value3=pImgData0[this->Image->channels() * (this->Image->cols * row + column) ];
								value4=pImgData0[this->Image->channels() * (this->Image->cols * row + column)+1];
								value5=pImgData0[this->Image->channels() * (this->Image->cols * row + column)+2];
								if(value_out0>value3)delta_value0=value_out0-value3;
								else delta_value0=value3-value_out0;
								if(value_out1>value4)delta_value1=value_out1-value4;
								else delta_value1=value4-value_out1;
								if(value_out2>value5)delta_value2=value_out2-value5;
								else delta_value2=value5-value_out2;
								//cout<<delta_value0<<","<<delta_value1<<","<<delta_value2<<endl;
								if( delta_value0<THRESHOLD && delta_value1<THRESHOLD  && delta_value2<THRESHOLD){
									count_column_equals++;
									//cout<<"*";
								}
								
							
							  if(column>x*tile_size+tile_size/5 && column<(x+1)*tile_size-tile_size/5 && count_column_equals>tile_size-tile_size/5){
								row=(y+1)*tile_size;
								//vector<int> coords;
								//coords.push_back(x);
								//coords.push_back(y);
								//list_tiles.push_back(coords);
								tile_matrix[x*N+y]+=1;	
								}
							}
							count_tile_equals+=count_column_equals;
							
							
					}
					//cout<<"count_tile_equals:"<<count_tile_equals<<endl;
					if(count_tile_equals>count_threshold){
							//vector<int> coords;
							//coords.push_back(x);
							//coords.push_back(y);
							//list_tiles.push_back(coords);
							//cout<<"x:"<<x<<"y:"<<y<<endl;
							tile_matrix[x*N+y]+=1;		
						}
					
				}
				
			}
			}
		

	}
	 /**Fill Rectangles from returned vector list_tile_size of rectangles from grid */
   
	for (int y = 0; y < N; y++){
		for (int x = 0; x < M; x++){
		
			if(tile_matrix[x*N+y]>0)
				rectangle(frame,Point(x*tile_size,y*tile_size),Point((x+1)*tile_size,(y+1)*tile_size),Scalar(0,0,0),-1);
				
			}
    }
	imshow("MyVideo", frame); ///show the frame in "MyVideo" window
	
	delete [] tile_matrix;
	}
	
	
	
	/** Save Tiles
	 * 
	 * 
	 */
	void Save_Tiles(int tile_size){
		
		int M=this->dWidth/tile_size;
		int N=this->dHeight/tile_size;
	    int counter_list=0;
		for (int y = 0; y < N; y++){
			for (int x = 0; x < M; x++){
				imwrite("tile_feature"+to_string(counter_list)+".png",*(list_tile_features[counter_list]));
				counter_list++;
				}
			}
	}
	
	void Create_Tiles(int tile_size){
		Mat frame=this->Image->clone();
		
		unsigned char *pImgData1 = (unsigned char *)frame.data;
		int tile_area=tile_size*tile_size;
		int count_threshold=(1.0-factor_tile_size)*tile_area;
		//cout<<count_threshold<<endl;
		//vector<vector<int>> list_tiles;
		int M=this->dWidth/tile_size;
		int N=this->dHeight/tile_size;
		cout<<"N:"<<N<<"M:"<<M<<endl;
		for (int y = 0; y < N; y++){
			for (int x = 0; x < M; x++){
				Mat *tile=new Mat(tile_size,tile_size,CV_8UC3);
				unsigned char *pImgData0 = (unsigned char *)tile->data;
				int tile_row=0;
				for (int row = y*tile_size; row < (y+1)*tile_size; row++){
					int tile_column=0;
					for (int column = x*tile_size; column < (x+1)*tile_size; column++){
							
					        
						    pImgData0[tile->channels() * (tile->cols * tile_row + tile_column) ]= pImgData1[frame.channels() * (frame.cols * row + column) ];
							pImgData0[tile->channels() * (tile->cols * tile_row + tile_column) +1] = pImgData1[frame.channels() * (frame.cols * row + column)+1];
							pImgData0[tile->channels() * (tile->cols * tile_row + tile_column) +2] = pImgData1[frame.channels() * (frame.cols * row + column)+2];
						
						tile_column++;
						}
					
					tile_row++;
					}
				list_tile_features.push_back(tile);
				}
			
			}
	
	
	
	}
	
	
	void Colorise(float value0,float value1,float value2,int *value_out0,int *value_out1,int *value_out2){
		
		if( value0>value1 && value0>value2){
			if(value1>value2){
				(*value_out0)=255;
				(*value_out1)=(unsigned char)(255*(value2-value1)/(value0-value1));
				(*value_out2)=0;
				}
			else{
				(*value_out0)=255;
				(*value_out2)=(unsigned char)(255*(value0-value1)/(value2-value1));
				(*value_out1)=0;
				}
		}
		else if(value1>value0 && value1>value2){
			if(value0>value2){
				(*value_out1)=255;
				(*value_out0)=(unsigned char)(255*(value2-value1)/(value0-value1));
				(*value_out2)=0;
				}
			else{
				(*value_out1)=255;
				(*value_out2)=(unsigned char)(255*(value0-value1)/(value2-value1));
				(*value_out0)=0;
				}

		}
		else if(value2>value0 && value2>value1){
			if(value0>value1){
				(*value_out2)=255;
				(*value_out0)=(unsigned char)(255*(value2-value1)/(value0-value1));
				(*value_out1)=0;
				}
			else{
				(*value_out2)=255;
				(*value_out1)=(unsigned char)(255*(value0-value1)/(value2-value1));
				(*value_out0)=0;
				}
		
		}
		
		
		
	}
	
	
	void Colorise2(float value0,float value1,float value2,int *value_out0,int *value_out1,int *value_out2){
			if(value0>value1){
					if(value2>value1){
						if(value0>value2){
							(*value_out0)=255;
							(*value_out2)=(unsigned char)(255*(value2-value1)/(value0-value1));
							(*value_out1)=0;
						}
						else{
							(*value_out2)=255;
							(*value_out0)=(unsigned char)(255*(value0-value1)/(value2-value1));
							(*value_out1)=0;
						}
					}
					else{
						if(value0>value2){
							(*value_out0)=255;
							(*value_out1)=(unsigned char)(255*(value1-value2)/(value0-value2));
							(*value_out2)=0;
							
						}
						else{
							(*value_out1)=255;
							(*value_out0)=(unsigned char)(255*(value0-value2)/(value1-value2));
							(*value_out2)=0;
							
						}						
					}
				}
				else {//value1 larger than value0
					if(value2>value0){
						if(value1>value2){
							(*value_out1)=255;
							(*value_out2)=(unsigned char)(255*(value2-value1)/(value0-value1));
							(*value_out0)=0;
								
						}
						else{
							(*value_out2)=255;
							(*value_out1)=(unsigned char)(255*(value0-value1)/(value2-value1));
							(*value_out0)=0;
							
						}					
					}
					else{//value0 is larger than value2 
						if(value1>value2){
							(*value_out1)=255;
							(*value_out2)=(unsigned char)(255*(value2-value1)/(value0-value1));
							(*value_out0)=0;
							
						}
						else{
							(*value_out2)=255;
							(*value_out1)=(unsigned char)(255*(value0-value1)/(value2-value1));
							(*value_out0)=0;
						
						}				
					}					
					
					
				}
		}

};

int main(int argc, char* argv[])
{
    

   
	BackGroundRemover BGR=BackGroundRemover(1280,720);

	cout<<"start loop"<<endl;
    while (1)
    {


        BGR.Show();
		/**Process grid with tile width of tile_size and store in vector, selected rectangles, from grid */

        BGR.Process(TILESIZE);
      

        
		int k=waitKey(30);
        if ( k%256== 27) ///wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
        else if (k%256==32){
		    
			BGR.UpdateBackground();
			//BGR.Create_Tiles(TILESIZE);
			//BGR.Save_Tiles(TILESIZE);
	   }
    }
    return 0;

}
