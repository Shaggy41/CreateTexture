// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Color.h"
#include "FileHelper.h"
#include "EngineUtils.h"
#include "Engine/Texture2D.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidFile.h"
#endif

#include "MyGameInstance.generated.h"

/*
* DELEGATES
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayLevelFromImage);


/**
 * 
 */
UCLASS()
class CREATETEXTURE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	//=================================================================
	// PROPERTIES
	//=================================================================
public:
	//-------------------------------------------------------------------
	/**
	* [public]
	* Allow change width, height and format pixel for creating texture. If false then width and height are gets from selected image and FormatPixel is PF_B8G8R8A8.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCustom;

	//-------------------------------------------------------------------
	/**
	* [public]
	* Output Message for extra information.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;

	//-------------------------------------------------------------------
	/**
	* [public]
	* The Height for texture.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HeightTex;

	//-------------------------------------------------------------------
	/**
	* [public]
	* The Width for texture.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WidthTex;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AbsolutePath;

	//=================================================================
	// CONSTRUCTOR
	//=================================================================
public:
	//-------------------------------------------------------------------
	/**
	* [public]
	* Base constructor with default value for properties
	*/
	UMyGameInstance();
	
	//=================================================================
	// METHODS
	//=================================================================
public:
	//-------------------------------------------------------------------
	/**
	* [public]
	* Load image from file and set to the texture.
	*/
	UFUNCTION(BlueprintCallable)
	UTexture2D * LoadImageFromFile(
		UPARAM(ref) const FString & FilePath
	);

	//-------------------------------------------------------------------
	/**
	* [public]
	* Detect image extension from file name.
	*/
	EImageFormat DetectImageExtension(
		const FString & FilePath
	);

	//-------------------------------------------------------------
	/**
	* [public]
	* Delegate called in FilesBrowser after choose the correct image.
	*
	* @see PlayLevelFromImage()
	*/
	UPROPERTY(BlueprintAssignable)
	FOnPlayLevelFromImage OnPlayLevelFromImage;

#if PLATFORM_ANDROID
	static jmethodID OpenGalleryJMethod;
	static jmethodID CloseApplicationJMethod;
	static jmethodID GetImagePathJMethod;

	//UFUNCTION()
	void OnActivityResult(
		JNIEnv* jenv,
		jobject thiz,
		jobject activity,
		jint requestCode,
		jint resultCode,
		jobject data
	);
#endif

	//-------------------------------------------------------------
	/**
	* [public]
	* Get root path.
	*/
	UFUNCTION(BlueprintCallAble)
	FString GetRootPath();

	//-------------------------------------------------------------------
	/**
	* [public]
	* Open files browser(Desktop) or gallery(Android)
	*/
	UFUNCTION(BlueprintCallable)
	void FilesBrowser();

	//=================================================================
	// PIXEL FORMAT
	//=================================================================
protected:
	//-------------------------------------------------------------------
	/**
	* [protected]
	* PixelFormat for creating texture.
	*/
	EPixelFormat PixelFormat;

public:
	//-------------------------------------------------------------------
	/**
	* [public]
	* Set PixelFormat from int32 value to EPixelFormat.
	*
	* @param PixelFormatInt The value must be between 0 and PF_MAX (64), otherwise, it will be set to the default value PF_B8G8R8A8.
	*/
	UFUNCTION(BlueprintCallable, Category = "PixelFormat")
	void SetPixelFormat(
		int32 PixelFormatInt
	);

	//=================================================================
	// RGBFormat
	//=================================================================
protected:
	//-------------------------------------------------------------------
	/**
	* [protected]
	* RGBFormat for a readed image from file.
	*/
	ERGBFormat RGBFormat;

public:
	//-------------------------------------------------------------------
	/**
	* [public]
	* Set RGBFormat from int32 value to ERGBFormat.
	*
	*@param RGBFormatInt The value must be between -1 and 2, otherwise, it will be set to the default value BGRA.
	*/
	UFUNCTION(BlueprintCallable, Category = "RGBFormat")
	void SetRGBFormat(
		int32 RGBFormatInt
	);
};
