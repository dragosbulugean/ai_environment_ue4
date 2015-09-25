// Fill out your copyright notice in the Description page of Project Settings.

#include "kortex_environment.h"
#include "KViewportClient.h"

//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>

#define KSize 1024

UKViewportClient::UKViewportClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	toggleBroadcasting = true;

	ZMQContext = zmq_init(1);
	if (ZMQContext == 0) {
		printf("Error zmq_init: '%s'\n", zmq_strerror(errno));
	}

	ZMQSocket = zmq_socket(ZMQContext, ZMQ_PUSH);
	if (ZMQSocket == 0) {
		printf("Error zmq_socket: '%s'\n", zmq_strerror(errno));
	}
	int val = 65535;
	zmq_setsockopt(ZMQSocket, ZMQ_MAXMSGSIZE, &val, sizeof(val));
	auto result = zmq_connect(ZMQSocket, "tcp://127.0.0.1:4440");
	if (result == 0) {
		printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
	}
}

void UKViewportClient::useFrames(){

	//if (!Viewport) return;
	FVector2D viewportSize;
}

void UKViewportClient::Draw(FViewport * Viewport, FCanvas * SceneCanvas)
{
	
	Super::Draw(Viewport, SceneCanvas);
	

	FIntPoint Size = Viewport->GetSizeXY();
	TArray<FColor>* pix = new TArray<FColor>();
	int width = Size.X,
		height = Size.Y,
		DataLength = width * height,
		bufferSize = DataLength * 3;
	Viewport->ReadPixels(*pix, FReadSurfaceDataFlags(),
		FIntRect(0, 0, width, height));
	FColor* data = pix->GetData();
	
	int rc = zmq_send(ZMQSocket, data, DataLength * sizeof(FColor), 0);

	if (rc == -1) {
		printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
	}
	/*uint8* frameBuffer = new uint8[bufferSize];

	for (auto i = 0; i < DataLength; i++)
	{
		frameBuffer[i * 3] = data[i].R;
		frameBuffer[i * 3 + 1] = data[i].G;
		frameBuffer[i * 3 + 2] = data[i].B;
		pix++;
	}

	zmq_msg_t msg;
	void *hint = NULL;

	int rc = zmq_msg_init_data(&msg, frameBuffer, bufferSize, &my_free_viewport, hint);
	
	rc = zmq_sendmsg(ZMQSocket, &msg, 0);*/

	//delete[] frameBuffer;

	////UE_LOG(Victory,Warning,TEXT("VICTORY GAME VIEWPORT Ticking!"));

	////Take Screen shot?
	//if (toggleBroadcasting)
	//{
	//	Viewport->ReadPixels(*pix, FReadSurfaceDataFlags(),
	//		FIntRect(0, 0, width, height));
	//	FColor* data = pix->GetData();

	//	using namespace cv;
	//	using namespace std;

	//	Mat img(height, width, CV_8UC3); //create an image ( 3 channels) 

	//	if (img.empty()) //check whether the image is loaded or not
	//	{
	//		cout << "ERROR : Image cannot be loaded..!!" << endl;
	//	}
	//	for (int i = 0; i < height; i++)
	//	{
	//		for (int j = 0; j < width; j++)
	//		{
	//			FColor currentPx = data[i*width + j];   //get pixel
	//			Vec3b px(currentPx.B, currentPx.G, currentPx.R);  //create Vec3b pixel ( BGR format ) 
	//			img.at<Vec3b>(i, j) = px;   //set image pixel
	//		}
	//	}

	//namedWindow("Broadcasting", CV_WINDOW_AUTOSIZE); //create a window with the name "Broadcasting"
	//imshow("Broadcasting", img); //display the image which is stored in the 'img' in the "Broadcasting" window

	//UE_LOG(LogTemp, Warning, TEXT("Broadcasting"));
	//}
}

void my_free_viewport(void *data, void *hint) { free(data); }