#include <pch.h>

#include "Explorer.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <axt/core/App.h>

#define AXT_EX_THROW_FILE_ERROR() AXT_WARN("FILE NOT FOUND")

namespace axt
{

	//inline static const char* gErrorFileMsg{ "FILE_NOT_OPENED" };

	std::string explorer::OpenFile(const char* filter)
	{
		OPENFILENAMEA openFile{};
		CHAR fileSize[260]{ 0 };
		ZeroMemory(&fileSize, sizeof(OPENFILENAMEA));
		openFile.lStructSize = sizeof(OPENFILENAMEA);
		openFile.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow()));
		openFile.lpstrFile = fileSize;
		openFile.nMaxFile = sizeof(fileSize);
		openFile.lpstrFilter = filter;
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		AXT_EX_THROW_FILE_ERROR();
		return {};
	}

	std::string explorer::SaveFile(const char* filter)
	{
		OPENFILENAMEA openFile{};
		CHAR fileSize[260]{ 0 };
		ZeroMemory(&fileSize, sizeof(OPENFILENAMEA));
		openFile.lStructSize = sizeof(OPENFILENAMEA);
		openFile.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow()));
		openFile.lpstrFile = fileSize;
		openFile.nMaxFile = sizeof(fileSize);
		openFile.lpstrFilter = filter;
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		AXT_EX_THROW_FILE_ERROR();
		return {};
	}

}
