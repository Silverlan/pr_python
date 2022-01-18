#include <pragma/pragma_module.hpp>
#include <string>
#include <memory>
#include <Python.h>
#include <sharedutils/util_string.h>

extern "C"
{
	bool PRAGMA_EXPORT pr_py_run(const char *code)
	{
		return PyRun_SimpleString(code) == 0;
	}
	bool PRAGMA_EXPORT pr_py_exec(const char *fileName,uint32_t argc,const char **argv)
	{
		std::vector<std::wstring> wargs;
		std::vector<wchar_t*> wargsp;
		wargs.resize(argc);
		wargsp.resize(argc);
		for(auto i=decltype(argc){0u};i<argc;++i)
		{
			wargs[i] = ustring::string_to_wstring(argv[i]);
			wargsp[i] = wargs[i].data();
		}
		// Note: Binary mode is intentional ( https://docs.python.org/3/c-api/veryhigh.html#c.PyRun_SimpleFileExFlags )
		std::unique_ptr<FILE,void(*)(FILE*)> f {fopen(fileName,"rb"),[](FILE *f) {
			if(!f)
				return;
			fclose(f);
		}};
		if(!f)
			return false;
		PySys_SetArgv(wargsp.size(),wargsp.data());
		return PyRun_SimpleFile(f.get(),fileName) == 0;
	}
	bool PRAGMA_EXPORT pragma_attach(std::string &outErr)
	{
		Py_Initialize();
		return true;
	}
	void PRAGMA_EXPORT pragma_detach()
	{
		Py_Finalize();
	}
}
