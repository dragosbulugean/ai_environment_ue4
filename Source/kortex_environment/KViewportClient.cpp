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
}


void UKViewportClient::useFrames(){

	//if (!Viewport) return;
	FVector2D viewportSize;
}

void UKViewportClient::Draw(FViewport * Viewport, FCanvas * SceneCanvas)
{
	
	Super::Draw(Viewport, SceneCanvas);

	//FIntPoint Size = Viewport->GetSizeXY();
	//TArray<FColor>* pix = new TArray<FColor>();
	//int width = Size.X;
	//int height = Size.Y;
	//Viewport->ReadPixels(*pix, FReadSurfaceDataFlags(),
	//	FIntRect(0, 0, width, height));


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

