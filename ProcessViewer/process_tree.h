#include <sstream>
#include <string>
#include <map>

#include <TlHelp32.h>
#include <Windows.h>

class process
{
public:
	process()
		:_process_name(L""), _ppid(0), _pid(0), _creation_time(0), _killed(false)
	{
	}

	process(_In_ const wchar_t* process_name, _In_ DWORD ppid, _In_ DWORD pid, _In_ uint64_t creation_time, _In_ bool killed)
		:_process_name(process_name), _ppid(ppid), _pid(pid), _creation_time(creation_time), _killed(killed)
	{
	}

	std::wstring&	process_name()	{ return _process_name; }
	DWORD			ppid()			{ return _ppid; }
	DWORD			pid()			{ return _pid; }
	uint64_t		creation_time() { return _creation_time; }
	bool			killed()		{ return _killed; }

private:
	std::wstring	_process_name;
	DWORD			_ppid;
	DWORD			_pid;
	uint64_t		_creation_time;
	bool			_killed;
};

typedef std::map< DWORD, process >	process_map;
typedef bool(*fnproc_tree_callback)(_In_ process& process_info, _In_ DWORD_PTR callback_tag);

class cprocess_tree
{
public:
	bool	clear_process_tree() { _proc_map.clear(); }
	bool	build_process_tree();

	DWORD			find_process(_In_ const wchar_t* process_name);
	const wchar_t*	get_process_name(_In_ DWORD pid);

	void	iterate_process(_In_ fnproc_tree_callback callback, _In_ DWORD_PTR callback_tag);
	void	iterate_process_tree(_In_ DWORD root_pid, _In_ fnproc_tree_callback, _In_ DWORD_PTR callback_tag);

	void	print_process_tree(_In_ DWORD root_pid);
	void	print_process_tree(_In_ const wchar_t* root_process_name);

	bool	kill_process(_In_ DWORD root_pid);
	bool	kill_process(_In_ const wchar_t* process_name);

	bool	kill_process_tree(_In_ DWORD root_pid);

private:
	process_map _proc_map;
};