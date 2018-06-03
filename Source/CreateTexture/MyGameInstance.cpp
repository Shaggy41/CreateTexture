// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

#include "Engine.h"

#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MAC
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#endif

#if PLATFORM_ANDROID
jmethodID UMyGameInstance::OpenGalleryJMethod;
//jmethodID UMyGameInstance::CloseApplicationJMethod;
jmethodID UMyGameInstance::GetImagePathJMethod;
#endif

extern FString GExternalFilePath;


UMyGameInstance::UMyGameInstance()
	: Super()
{
	RGBFormat = ERGBFormat::BGRA;
	PixelFormat = EPixelFormat::PF_B8G8R8A8;
	bCustom = false;
	WidthTex = HeightTex = 64;

#if PLATFORM_WINDOWS
	AbsolutePath = "C:/";
#elif PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		OpenGalleryJMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_OpenGallery", "()V", false);
		//CloseApplicationJMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CloseApplication", "()V", false);
		GetImagePathJMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetImagePath", "()Ljava/lang/String;", false);
	}

	FJavaWrapper::OnActivityResultDelegate.AddUObject(this, &UMyGameInstance::OnActivityResult);

#endif
}


//=============================================================
UTexture2D * UMyGameInstance::LoadImageFromFile(
	const FString & FilePath
)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(DetectImageExtension(FilePath));

	Message = "";
	//-------------------------------------------------------------------
	//Load From File
	TArray<uint8> RawFileData;

	if (!FFileHelper::LoadFileToArray(RawFileData, *FilePath))
	{
		Message = "Can't load from file";

		return nullptr;
	}
	//-------------------------------------------------------------------

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArray<uint8>* UncompressedBGRA = NULL;

		// Uncompress selected image.
		if (ImageWrapper->GetRaw(RGBFormat, 8, UncompressedBGRA))
		{
			//-------------------------------------------------------------------
			// Create empty (white) texture 2D.
			UTexture2D * LoadedImage = nullptr;

			if (bCustom)
			{
				LoadedImage = UTexture2D::CreateTransient(
					WidthTex, HeightTex,
					PixelFormat);
			}
			else
			{
				LoadedImage = UTexture2D::CreateTransient(
					ImageWrapper->GetWidth() , ImageWrapper->GetHeight(),
					PF_B8G8R8A8);
			}
			//-------------------------------------------------------------------

			//-------------------------------------------------------------------
			// Fill texture with pixels.
			if (LoadedImage)
			{
				FByteBulkData * RawData = &LoadedImage->PlatformData->Mips[0].BulkData;
				uint8 * TextureData = (uint8 *)RawData->Lock(LOCK_READ_WRITE);
				
				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), FMath::Min<int32>(RawData->GetElementCount(), UncompressedBGRA->Num()));

				RawData->Unlock();
				Message = "Done :D\n" 
					+ FString::Printf(L"Image in Bytes: %d\n", RawData->GetElementCount())
					+ FString::Printf(L"Readed number of the Bytes: %d", UncompressedBGRA->Num());
				//Update!
				LoadedImage->UpdateResource();

				return LoadedImage;			
			}
			else
			{
				Message = "New Texture error";
			}
			//-------------------------------------------------------------------
		}
		else
		{
			Message = "GetRaw error";
		}
	}
	else
	{
		Message = "ImageWrapper isn't valid.";
	}

	return nullptr;
}


//=============================================================
EImageFormat UMyGameInstance::DetectImageExtension(
	const FString & FilePath
)
{
	int32 LastDot = 0;

	FilePath.FindLastChar('.', LastDot);

	FString Ext = FilePath.RightChop(LastDot + 1);

	if (Ext.Equals("PNG", ESearchCase::IgnoreCase))
	{
		return EImageFormat::PNG;
	}

	if (Ext.Equals("BMP", ESearchCase::IgnoreCase))
	{
		return EImageFormat::BMP;
	}

	if (Ext.Equals("JPG", ESearchCase::IgnoreCase) || Ext.Equals("JPEG", ESearchCase::IgnoreCase))
	{
		return EImageFormat::JPEG;
	}

	return EImageFormat::Invalid;
}


//=============================================================
#if PLATFORM_ANDROID

void UMyGameInstance::OnActivityResult(
	JNIEnv* jenv,
	jobject thiz,
	jobject activity,
	jint requestCode,
	jint resultCode,
	jobject data
)
{
	if ((int32)requestCode == 111) // RESULT_LOAD_IMAGE (defined by my self in .xml file)
	{
		if ((int32)resultCode == -1) // RESULT_OK
		{
			jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(jenv, FJavaWrapper::GameActivityThis, UMyGameInstance::GetImagePathJMethod);
			// Convert jstring to FString
			const char* JavaChars = jenv->GetStringUTFChars(JavaString, 0);
			FString ImagePath = FString(UTF8_TO_TCHAR(JavaChars));
			// release string
			jenv->ReleaseStringUTFChars(JavaString, JavaChars);
			jenv->DeleteLocalRef(JavaString);

			AbsolutePath = (GetRootPath() + ImagePath);
		}
	}
}
#endif


//=============================================================
FString UMyGameInstance::GetRootPath()
{
	FString Path = "";
#if PLATFORM_ANDROID
	TArray<FString> Folders;

	GExternalFilePath.ParseIntoArray(Folders, TEXT("/"));

	for (FString Folder : Folders)
	{
		Path += FString("/..");
	}
#endif

	return Path;
}


//=============================================================
void UMyGameInstance::FilesBrowser()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MAC
	IDesktopPlatform * DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TArray<FString> ResultString;

		if (DesktopPlatform->OpenFileDialog(
			GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle(),
			L"Choose image",
			AbsolutePath,
			L"",
			L"Image|*.jpg;*.jpeg;*.png;*.bmp",
			EFileDialogFlags::None,
			ResultString))
		{
			if (ResultString.Num() > 0)
			{
				AbsolutePath = ResultString[0];
			}
		}
	}

	// Open gallery on the Android
#elif PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UMyGameInstance::OpenGalleryJMethod);
	}
#endif
}


//=================================================================
// PIXEL FORMAT
//=================================================================

//=============================================================
void UMyGameInstance::SetPixelFormat(
	int32 PixelFormatInt
)
{
	if (PixelFormatInt < 0 || PixelFormatInt >(int32)EPixelFormat::PF_MAX)
	{
		PixelFormat = EPixelFormat::PF_B8G8R8A8;
	}
	else
	{
		PixelFormat = (EPixelFormat)PixelFormatInt;
	}
}


//=================================================================
// RGBFormat
//=================================================================

//=================================================================
void UMyGameInstance::SetRGBFormat(
	int32 RGBFormatInt
)
{
	if (RGBFormatInt < -1 || RGBFormatInt > 2)
	{
		RGBFormat = ERGBFormat::BGRA;
	}
	else
	{
		RGBFormat = (ERGBFormat)RGBFormatInt;
	}
}