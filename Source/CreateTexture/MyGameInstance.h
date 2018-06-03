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

/*
* Copy from RenderUtils.cpp

FPixelFormatInfo	GPixelFormats[PF_MAX] =
{
// Name						BlockSizeX	BlockSizeY	BlockSizeZ	BlockBytes	NumComponents	PlatformFormat	Supported		UnrealFormat

{ TEXT("unknown"),			0,			0,			0,			0,			0,				0,				0,				PF_Unknown },
{ TEXT("A32B32G32R32F"),	1,			1,			1,			16,			4,				0,				1,				PF_A32B32G32R32F },
{ TEXT("B8G8R8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_B8G8R8A8 },
{ TEXT("G8"),				1,			1,			1,			1,			1,				0,				1,				PF_G8 },
{ TEXT("G16"),				1,			1,			1,			2,			1,				0,				1,				PF_G16 },
{ TEXT("DXT1"),				4,			4,			1,			8,			3,				0,				1,				PF_DXT1 },
{ TEXT("DXT3"),				4,			4,			1,			16,			4,				0,				1,				PF_DXT3 },
{ TEXT("DXT5"),				4,			4,			1,			16,			4,				0,				1,				PF_DXT5 },
{ TEXT("UYVY"),				2,			1,			1,			4,			4,				0,				0,				PF_UYVY },
{ TEXT("FloatRGB"),			1,			1,			1,			4,			3,				0,				1,				PF_FloatRGB },
{ TEXT("FloatRGBA"),		1,			1,			1,			8,			4,				0,				1,				PF_FloatRGBA },
{ TEXT("DepthStencil"),		1,			1,			1,			4,			1,				0,				0,				PF_DepthStencil },
{ TEXT("ShadowDepth"),		1,			1,			1,			4,			1,				0,				0,				PF_ShadowDepth },
{ TEXT("R32_FLOAT"),		1,			1,			1,			4,			1,				0,				1,				PF_R32_FLOAT },
{ TEXT("G16R16"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16 },
{ TEXT("G16R16F"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16F },
{ TEXT("G16R16F_FILTER"),	1,			1,			1,			4,			2,				0,				1,				PF_G16R16F_FILTER },
{ TEXT("G32R32F"),			1,			1,			1,			8,			2,				0,				1,				PF_G32R32F },
{ TEXT("A2B10G10R10"),      1,          1,          1,          4,          4,              0,              1,				PF_A2B10G10R10 },
{ TEXT("A16B16G16R16"),		1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16 },
{ TEXT("D24"),				1,			1,			1,			4,			1,				0,				1,				PF_D24 },
{ TEXT("PF_R16F"),			1,			1,			1,			2,			1,				0,				1,				PF_R16F },
{ TEXT("PF_R16F_FILTER"),	1,			1,			1,			2,			1,				0,				1,				PF_R16F_FILTER },
{ TEXT("BC5"),				4,			4,			1,			16,			2,				0,				1,				PF_BC5 },
{ TEXT("V8U8"),				1,			1,			1,			2,			2,				0,				1,				PF_V8U8 },
{ TEXT("A1"),				1,			1,			1,			1,			1,				0,				0,				PF_A1 },
{ TEXT("FloatR11G11B10"),	1,			1,			1,			4,			3,				0,				0,				PF_FloatR11G11B10 },
{ TEXT("A8"),				1,			1,			1,			1,			1,				0,				1,				PF_A8 },
{ TEXT("R32_UINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_UINT },
{ TEXT("R32_SINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_SINT },

// IOS Support
{ TEXT("PVRTC2"),			8,			4,			1,			8,			4,				0,				0,				PF_PVRTC2 },
{ TEXT("PVRTC4"),			4,			4,			1,			8,			4,				0,				0,				PF_PVRTC4 },

{ TEXT("R16_UINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_UINT },
{ TEXT("R16_SINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_SINT },
{ TEXT("R16G16B16A16_UINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UINT },
{ TEXT("R16G16B16A16_SINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SINT },
{ TEXT("R5G6B5_UNORM"),     1,          1,          1,          2,          3,              0,              1,              PF_R5G6B5_UNORM },
{ TEXT("R8G8B8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8 },
{ TEXT("A8R8G8B8"),			1,			1,			1,			4,			4,				0,				1,				PF_A8R8G8B8 },
{ TEXT("BC4"),				4,			4,			1,			8,			1,				0,				1,				PF_BC4 },
{ TEXT("R8G8"),				1,			1,			1,			2,			2,				0,				1,				PF_R8G8 },

{ TEXT("ATC_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ATC_RGB },
{ TEXT("ATC_RGBA_E"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_E },
{ TEXT("ATC_RGBA_I"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_I },
{ TEXT("X24_G8"),			1,			1,			1,			1,			1,				0,				0,				PF_X24_G8 },
{ TEXT("ETC1"),				4,			4,			1,			8,			3,				0,				0,				PF_ETC1 },
{ TEXT("ETC2_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ETC2_RGB },
{ TEXT("ETC2_RGBA"),		4,			4,			1,			16,			4,				0,				0,				PF_ETC2_RGBA },
{ TEXT("PF_R32G32B32A32_UINT"),1,		1,			1,			16,			4,				0,				1,				PF_R32G32B32A32_UINT },
{ TEXT("PF_R16G16_UINT"),	1,			1,			1,			4,			4,				0,				1,				PF_R16G16_UINT },

// ASTC support
{ TEXT("ASTC_4x4"),			4,			4,			1,			16,			4,				0,				0,				PF_ASTC_4x4 },
{ TEXT("ASTC_6x6"),			6,			6,			1,			16,			4,				0,				0,				PF_ASTC_6x6 },
{ TEXT("ASTC_8x8"),			8,			8,			1,			16,			4,				0,				0,				PF_ASTC_8x8 },
{ TEXT("ASTC_10x10"),		10,			10,			1,			16,			4,				0,				0,				PF_ASTC_10x10 },
{ TEXT("ASTC_12x12"),		12,			12,			1,			16,			4,				0,				0,				PF_ASTC_12x12 },

{ TEXT("BC6H"),				4,			4,			1,			16,			3,				0,				1,				PF_BC6H },
{ TEXT("BC7"),				4,			4,			1,			16,			4,				0,				1,				PF_BC7 },
{ TEXT("R8_UINT"),			1,			1,			1,			1,			1,				0,				1,				PF_R8_UINT },
{ TEXT("L8"),				1,			1,			1,			1,			1,				0,				0,				PF_L8 },
{ TEXT("XGXR8"),			1,			1,			1,			4,			4,				0,				1,				PF_XGXR8 },
{ TEXT("R8G8B8A8_UINT"),	1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_UINT },
{ TEXT("R8G8B8A8_SNORM"),	1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_SNORM },

{ TEXT("R16G16B16A16_UINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UNORM },
{ TEXT("R16G16B16A16_SINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SNORM },
};
*/

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
	* When true then try copy pixels from selected file to the texture.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCopyPixelFromFile;

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
	* Create gray texture.
	*/
	UFUNCTION(BlueprintCallable)
	UTexture2D * CreateTexture();

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
