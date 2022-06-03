#include <iostream>
#include "ros/ros.h"
//#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"


#include <memory>

cv_bridge::CvImagePtr DepthImageData;
cv_bridge::CvImagePtr RgbImageData;
ros::Publisher mSensorsPublisher;
ros::Subscriber Activador;
ros::Publisher treePub;
ros::Publisher controlPub;
ros::Publisher objectPub;
ros::Publisher talkPub;
const int max_vals = 10;
double R_ = 0;
double G_ = 0;
double B_ = 0;
double Rs [max_vals];
double Gs [max_vals];
double Bs [max_vals];
int values_counter = 0;
float min_dist;
bool found_person;
bool act = false;
double now;
int fr = 10;
geometry_msgs::Pose2D pp;
std_msgs::String msg;
std_msgs::String object;
std_msgs::Bool found;
float xmax;
float xmin;
float ymax;
float ymin;

void activacionTree(const std_msgs::Bool::ConstPtr& clave)
{ 
  act = clave->data;
  if(act){
	  std::cout << "OBSERVADOR ACTIVADO\n";
  }
  else{
	  std::cout << "OBSERVADOR DESACTIVADO\n";
  }
}

double hacerMedia(double arr[])
{
	double result = 0;
	
	for(int i = 0;i < max_vals; i++){
		result = result + arr[i];
		//ROS_INFO("Media: %d", result);
	}
	result = result/max_vals;
	ROS_INFO("Media: %f", result);
	return result;
}


void callback_bbx(const sensor_msgs::ImageConstPtr& depth, const sensor_msgs::ImageConstPtr& rgb, const darknet_ros_msgs::BoundingBoxesConstPtr& boxes)
{
	if(act){
		DepthImageData = cv_bridge::toCvCopy(*depth, sensor_msgs::image_encodings::TYPE_32FC1);
		cv::Mat profundidadG_;
		/////////////////////
		//cv::GaussianBlur(mImageData->image,profundidadG_,cv::Size(33,33),0,0,0);
		
		cv::erode(DepthImageData->image, profundidadG_, cv::Mat());
		//////////////////////******************CAMBIOS*****************

		RgbImageData = cv_bridge::toCvCopy(*rgb, sensor_msgs::image_encodings::RGB16);
		//cv::Mat rgb = RgbImageData->image;
		//cv::erode(RgbImageData->image, rgb, cv::Mat());

		std::string tag = "person";
		
		float px_center=DepthImageData->image.cols/2;
		float py_center=DepthImageData->image.rows/2;
		// #############################
		//min_dist=3;
		min_dist=0.5;
		// #####################################
		found_person = false;
		//found_object = false;
		
		for(int i=0;i<boxes->bounding_boxes.size();i++)
		{
			if (boxes->bounding_boxes[i].Class == tag)
			{
				float px = (boxes->bounding_boxes[i].xmax + boxes->bounding_boxes[i].xmin) / 2;
				float py = 2 * (boxes->bounding_boxes[i].ymax + boxes->bounding_boxes[i].ymin) / 3;
				

				float dist = profundidadG_.at<float>(cv::Point(px, py)) * 0.001f;


				if (dist < min_dist)
				{
					min_dist = dist;
					//pp.x = dist;
					//float angle = (px - px_center)/(px_center);
					//pp.y = angle;
                    
					// #####################CAMBIOS_JORGE##########################
				
					int lado = 4 ; // mitad de lado 

					int x_i = px - lado ;      //      -------> X
					int y_i = py - lado ;         //   |
					int x_f = px + lado ;         //   |            Ejes en cv2 
					int y_f = py + lado ;         //   | 
                                                  //   v Y
					float r_me = 0 ;
					float g_me = 0 ;
					float b_me = 0 ;

					float n = ((lado )*2 +1 )*((lado )*2 +1 )  ; // numero de pixels en el area

                    for(int x = x_i ; x < x_f ; x++ ){
						for(int y = y_i ; y < y_f ;y++ ){

							cv::Vec3b vector_rgb = RgbImageData->image.at<cv::Vec3b>(cv::Point(x,y)); // saca los valores rgb del pixel x,y
                             r_me += vector_rgb[2] ;
							 g_me += vector_rgb[1] ;
                             b_me += vector_rgb[0] ;	
						}
					}

                    r_me = r_me / n ;
					g_me = g_me / n ;
					b_me = b_me / n ;

					
                    // #######################################################

					cv::Vec3b vector_rgb = RgbImageData->image.at<cv::Vec3b>(cv::Point(px,py));

					R_ = r_me;
					G_ = g_me;
					B_ = b_me;

					//pp.x = R_;
					//pp.y = G_;
					//pp.theta = B_;

					xmax = boxes->bounding_boxes[i].xmax;
					xmin = boxes->bounding_boxes[i].xmin;
					ymax = boxes->bounding_boxes[i].ymax;
					ymin = boxes->bounding_boxes[i].ymin;


					if( (R_ != 0 || G_ != 0 || B_ != 0 ) && values_counter < max_vals){
						Rs[values_counter] = R_;
						Gs[values_counter] = G_;
						Bs[values_counter] = B_;

						//ROS_INFO("R = %d", Rs[values_counter]);
						//ROS_INFO("G = %d", Gs[values_counter]);
						//ROS_INFO("B = %d", Bs[values_counter]);
						
						values_counter++;
						//ROS_INFO("values = %d", values_counter);
					}

					found_person = true;

					}
			}
		}
		if (found_person){
			if(values_counter < max_vals)
			{
			std::stringstream ss;
			ss << "RUNNING";
			msg.data = ss.str();
			treePub.publish(msg);
			//Solo para tests
			//mSensorsPublisher.publish(pp);
			}
			else{

				pp.x = hacerMedia(Rs);
				pp.y = hacerMedia(Gs);
				pp.theta = hacerMedia(Bs);


				std::stringstream ss;
				ss << "SUCCESS";
				msg.data = ss.str();
				treePub.publish(msg);

				std::stringstream dialog;
				dialog << "Welcome";
				msg.data = dialog.str();
				talkPub.publish(msg);

				mSensorsPublisher.publish(pp);
				objectPub.publish(object);

				values_counter = 0;
				act = false;
			}
		}
		else
		{
			std::stringstream ss;
			ss << "FAILURE";
			msg.data = ss.str();
			treePub.publish(msg);
		}
	}
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "ObservadorPersonas");

	ros::NodeHandle nh;

	message_filters::Subscriber<sensor_msgs::Image> image_depth_sub(nh, "/camera/depth/image_raw", fr);
	message_filters::Subscriber<sensor_msgs::Image> image_rgb_sub(nh, "/camera/rgb/image_raw", fr);
  	message_filters::Subscriber<darknet_ros_msgs::BoundingBoxes> bbx_sub(nh, "/darknet_ros/bounding_boxes", fr);
	

 	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image, darknet_ros_msgs::BoundingBoxes> Sync;
  	message_filters::Synchronizer<Sync> sync_bbx(Sync(10), image_depth_sub, image_rgb_sub, bbx_sub);

  	sync_bbx.registerCallback(boost::bind(&callback_bbx, _1, _2, _3));

	mSensorsPublisher = nh.advertise<geometry_msgs::Pose2D>("/person_data", fr);
	Activador = nh.subscribe("/control_observador", fr, activacionTree);
	treePub = nh.advertise<std_msgs::String>("/status_observador", fr);
	objectPub = nh.advertise<std_msgs::String>("/object_data", fr);
	talkPub = nh.advertise<std_msgs::String>("/msg_receive", fr);
	controlPub = nh.advertise<std_msgs::Bool>("/reset_observador", fr);

	ros::spin();
}