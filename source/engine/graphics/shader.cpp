#include "shader.h"
#include "graphics.h"
#include "../engine.h"
#include "spirv_reflect.h"
#include <utils/compile_random.h>

#include <fstream>
#if _WIN32
	#include <shellapi.h>
	#pragma warning(disable : 6387)
#endif
namespace engine
{
	static AutoCVarString cVarShaderCompilerPath(
		"r.RHI.ShaderCompilerPath",
		"Shader compiler store path, relative to install path.",
		"RHI",
		"tool/glslc.exe",
		CVarFlags::ReadOnly
	);

	static AutoCVarBool cVarShaderSkipIfCacheExist(
		"r.RHI.ShaderSkipIfCacheExist",
		"Shader skip if shader cache exist.",
		"RHI",
		true,
		CVarFlags::ReadOnly
	);

	const std::array<ShaderStageInfoForCompile, kMaxShaderStage> kShaderCompileInfos =
	{ {
		{ L"vert", L"VERTEX_SHADER"  },
		{ L"frag", L"PIXEL_SHADER"   },
		{ L"comp", L"COMPUTE_SHADER" },
	} };

	void ShaderCache::releaseModule(VkShaderModule shader)
	{
	}

	VkShaderModule ShaderCache::createShaderModule(const ShaderVariant& variant, bool bRecompile) const
	{
		// ����Ҳ���ǵ�һ��������Ϸʱ����Ҫ�ȴ�������ɫ����ԭ��
		// �ִ�ͼ��API����ƶ��ǽ�shader���뾡��������ǰ�����
		// ��Ȼ��Ҫ������һ�����̣����Ͼ�����ú�������ʱ�Ϳ��ˣ���������ν
		constexpr size_t kCompileValue = Dynlec::CTRandomTimeSeed;

		const auto compilerPath = std::filesystem::absolute(cVarShaderCompilerPath.get());
		const auto shaderPath = std::filesystem::absolute(variant.m_path);
		const auto saveFolderPath = std::filesystem::absolute(kShaderCacheFolder);

		auto saveName = shaderPath.filename().wstring() + std::to_wstring(hashCombine(kCompileValue, variant.getHash()));
		const auto saveShaderPath = saveFolderPath / saveName;

		if (std::filesystem::exists(saveShaderPath)
			&& !bRecompile
			&& cVarShaderSkipIfCacheExist.get())
		{
			//
		}
		else 
		{
			LOG_RHI_TRACE("Compiling shader {0} with uuid {1}...", shaderPath.filename().string(), std::to_string(variant.getHash()));
			uint64_t compileResult = 0U;
#if _WIN32
			while (true)
			{
				const auto& exePath = compilerPath.wstring();
				const auto& defineInfo = kShaderCompileInfos[size_t(variant.m_shaderStage)];
			
				// �����г������
				std::wstring params =
					std::format(L"-fshader-stage={} --target-env=vulkan1.3 -D{}", defineInfo.name, defineInfo.basicDefined);

				for (const auto& macro : variant.m_macroSet)
				{
					params = std::format(L"{} -D{}", params, macro);
				}

#ifdef APP_DEBUG
				// Generate source level debug info for shader, use for nsight or renderdoc.
				if (cVarShaderSourceDebug.get())
				{
					params = std::format(L"{} -g", params);
				}
#endif
				for (const auto& keyValue : variant.m_keyMap)
				{
					params = std::format(L"{} -D{}={}", params, keyValue.first, std::to_wstring(keyValue.second));
				}

				params = std::format(L"{} {} -O -o {}", params, shaderPath.wstring(), saveShaderPath.wstring());
				{
					SHELLEXECUTEINFO ShExecInfo = { 0 };
					ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
					ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE;
					ShExecInfo.hwnd = NULL;
					ShExecInfo.lpVerb = NULL;
					ShExecInfo.lpFile = (LPCSTR)exePath.c_str();
					ShExecInfo.lpParameters = (LPCSTR)params.c_str();
					ShExecInfo.lpDirectory = NULL;
					ShExecInfo.nShow = SW_HIDE;
					ShExecInfo.hInstApp = NULL;
					ShellExecuteEx(&ShExecInfo);
					WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

					DWORD OutCompileResult = { 0 };
					GetExitCodeProcess(ShExecInfo.hProcess, &OutCompileResult);
					CloseHandle(ShExecInfo.hProcess);
					compileResult = (uint64_t)OutCompileResult;
				}

				if (compileResult)
				{
					MessageBox(NULL, (LPCSTR)L"Shader compile error, see log output, please fix me and recompile...",
						(LPCSTR)L"ShaderCompiler", MB_OK);
				}
				else
				{
					break;
				}
			}
#else
			#error "Still no imlement other platform compiler, fixed me!";
#endif
		}

		auto file = std::ifstream(saveShaderPath, std::ios::binary);
		if (file.bad())
		{
			LOG_RHI_FATAL("Open shader compile result file: {} failed.", utf8::utf16to8(saveShaderPath.u16string()));
			return nullptr;
		}

		file.seekg(0, std::ios::end);
		int length = (int)file.tellg();

		std::vector<char> opcodes(length);
		file.seekg(0, std::ios::beg);
		file.read((char*)opcodes.data(), length);

		VkShaderModuleCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ci.codeSize = length;
		ci.pCode = (uint32_t*)opcodes.data();

		VkShaderModule shaderModule;
		RHICheck(vkCreateShaderModule(getDevice(), &ci, nullptr, &shaderModule));

		return shaderModule;
	}
	VkShaderModule ShaderCache::getShader(const ShaderVariant& variant, bool reload, bool recompile)
	{
		return VkShaderModule();
	}
	void ShaderCache::release(bool bDeleteFiles)
	{
	}
	size_t ShaderVariant::getHash() const
	{
		return size_t();
	}
}