#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	///////////////////////////////////////////////////////////////////////////////
	// ApiSet v2

	typedef struct _API_SET_VALUE_ENTRY_REDIRECTION_V2
	{
		ULONG NameOffset;
		USHORT NameLength;
		ULONG ValueOffset;
		USHORT ValueLength;
	} API_SET_VALUE_ENTRY_REDIRECTION_V2, * PAPI_SET_VALUE_ENTRY_REDIRECTION_V2;

	typedef struct _API_SET_VALUE_ENTRY_V2
	{
		ULONG NumberOfRedirections;
		API_SET_VALUE_ENTRY_REDIRECTION_V2 Redirections[ANYSIZE_ARRAY];
	} API_SET_VALUE_ENTRY_V2, * PAPI_SET_VALUE_ENTRY_V2;

	typedef struct _API_SET_NAMESPACE_ENTRY_V2
	{
		ULONG NameOffset;
		ULONG NameLength;
		ULONG DataOffset; // ===> _API_SET_VALUE_ENTRY_V2
	} API_SET_NAMESPACE_ENTRY_V2, * PAPI_SET_NAMESPACE_ENTRY_V2;

	typedef struct _API_SET_NAMESPACE_V2
	{
		ULONG Version;
		ULONG Count;
		API_SET_NAMESPACE_ENTRY_V2 Array[ANYSIZE_ARRAY];
		typedef _API_SET_VALUE_ENTRY_V2 VALUE_ENTRY_TYPE;
	} API_SET_NAMESPACE_V2, * PAPI_SET_NAMESPACE_V2;

	///////////////////////////////////////////////////////////////////////////////
	// ApiSet v4

	typedef struct _API_SET_VALUE_ENTRY_REDIRECTION_V4
	{
		ULONG Flags;
		ULONG NameOffset;
		ULONG NameLength;
		ULONG ValueOffset;
		ULONG ValueLength;
	} API_SET_VALUE_ENTRY_REDIRECTION_V4, * PAPI_SET_VALUE_ENTRY_REDIRECTION_V4;

	typedef struct _API_SET_VALUE_ENTRY_V4
	{
		ULONG Flags;
		ULONG NumberOfRedirections;
		API_SET_VALUE_ENTRY_REDIRECTION_V4 Redirections[ANYSIZE_ARRAY];
	} API_SET_VALUE_ENTRY_V4, * PAPI_SET_VALUE_ENTRY_V4;

	typedef struct _API_SET_NAMESPACE_ENTRY_V4
	{
		ULONG Flags;
		ULONG NameOffset;
		ULONG NameLength;
		ULONG AliasOffset;
		ULONG AliasLength;
		ULONG DataOffset; // ===> _API_SET_VALUE_ENTRY_V4
	} API_SET_NAMESPACE_ENTRY_V4, * PAPI_SET_NAMESPACE_ENTRY_V4;

	typedef struct _API_SET_NAMESPACE_V4
	{
		ULONG Version;
		ULONG Size;
		ULONG Flags;
		ULONG Count;
		API_SET_NAMESPACE_ENTRY_V4 Array[ANYSIZE_ARRAY];
		typedef _API_SET_VALUE_ENTRY_V4 VALUE_ENTRY_TYPE;
	} API_SET_NAMESPACE_V4, * PAPI_SET_NAMESPACE_V4;

	///////////////////////////////////////////////////////////////////////////////
	// ApiSet v6

	typedef struct _API_SET_HASH_ENTRY_V6
	{
		ULONG Hash;
		ULONG Index;
	} API_SET_HASH_ENTRY_V6, * PAPI_SET_HASH_ENTRY_V6;

	typedef struct _API_SET_NAMESPACE_ENTRY_V6
	{
		ULONG Flags;
		ULONG NameOffset;
		ULONG NameLength;
		ULONG HashedLength;
		ULONG ValueOffset;
		ULONG ValueCount;
	} API_SET_NAMESPACE_ENTRY_V6, * PAPI_SET_NAMESPACE_ENTRY_V6;

	typedef struct _API_SET_VALUE_ENTRY_V6
	{
		ULONG Flags;
		ULONG NameOffset;
		ULONG NameLength;
		ULONG ValueOffset;
		ULONG ValueLength;
	} API_SET_VALUE_ENTRY_V6, * PAPI_SET_VALUE_ENTRY_V6;

	typedef struct _API_SET_NAMESPACE_V6
	{
		ULONG Version;
		ULONG Size;
		ULONG Flags;
		ULONG Count;
		ULONG EntryOffset;
		ULONG HashOffset;
		ULONG HashFactor;
	} API_SET_NAMESPACE_V6, * PAPI_SET_NAMESPACE_V6;

	///////////////////////////////////////////////////////////////////////////////

	typedef struct _API_SET_NAMESPACE
	{
		union
		{
			ULONG Version;
			API_SET_NAMESPACE_V2 ApiSetNameSpaceV2;
			API_SET_NAMESPACE_V4 ApiSetNameSpaceV4;
			API_SET_NAMESPACE_V6 ApiSetNameSpaceV6;
		};
	} API_SET_NAMESPACE, * PAPI_SET_NAMESPACE;

#ifdef __cplusplus
}
#endif

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>

PAPI_SET_NAMESPACE GetApiSetNamespace()
{
	ULONG ReturnLength;
	PROCESS_BASIC_INFORMATION ProcessInformation;
	PAPI_SET_NAMESPACE apiSetMap = NULL;

	//	Retrieve PEB address
	if (!NT_SUCCESS(NtQueryInformationProcess(
		GetCurrentProcess(),
		ProcessBasicInformation,
		&ProcessInformation,
		sizeof(PROCESS_BASIC_INFORMATION),
		&ReturnLength)))
	{
		return NULL;
	}

	//	Parsing PEB structure and locating api set map
	PPEB peb = static_cast<PPEB>(ProcessInformation.PebBaseAddress);
	apiSetMap = reinterpret_cast<PAPI_SET_NAMESPACE>(/*peb->ApiSetMap*/ *(int*)(((LONG_PTR)peb + 0x38)));

	return apiSetMap;
}

namespace std
{
	template <typename CharType>
	inline bool starts_with(
		const basic_string<CharType>& left,
		const basic_string<CharType>& right)
	{
		if (&left == &right)
			return true;
		const typename basic_string<CharType>::size_type big_size = left.size();
		const typename basic_string<CharType>::size_type small_size = right.size();
		const bool valid_ = (big_size >= small_size);
		const bool starts_with_ = (left.compare(0, small_size, right) == 0);
		return valid_ && starts_with_;
	}

	template <typename CharType>
	inline bool ends_with(const basic_string<CharType>& left, const basic_string<CharType>& right)
	{
		if (&left == &right)
			return true;
		const typename basic_string<CharType>::size_type big_size = left.size();
		const typename basic_string<CharType>::size_type small_size = right.size();
		const bool valid_ = (big_size >= small_size);
		const bool ends_with_ = (left.compare(big_size - small_size, small_size, right) == 0);
		return valid_ && ends_with_;
	}

	template <typename CharType>
	inline basic_string<CharType> to_lower(const basic_string<CharType>& name)
	{
		basic_string<CharType> value;
		std::transform(name.begin(), name.end(), std::back_inserter(value), std::tolower);
		return value;
	}
} // namespace std

typedef std::vector<std::wstring> ApiSetTarget;

class ApiSetSchema
{
public:
	std::vector<std::pair<std::wstring, ApiSetTarget>> m_allApiSets;

public:
	virtual std::vector<std::pair<std::wstring, ApiSetTarget>> GetAll() { return m_allApiSets; }
	virtual ApiSetTarget Lookup(std::wstring name) { return {}; }
};

class V2V4ApiSetSchema : public ApiSetSchema
{
public:
	virtual ApiSetTarget Lookup(std::wstring name)
	{
		// Force lowercase name
		std::transform(name.begin(), name.end(), std::back_inserter(name), std::tolower);

		if (name.substr(0, 4) == L"api-")
		{
			// TODO : check if ext- is not present on win7 and 8.1
			return {};
		}

		// remove "api-" or "ext-" prefix
		name = name.substr(4);

		// Note: The list is initially alphabetically sorted!!!
		size_t min = 0;
		auto max = m_allApiSets.size();
		while (min <= max)
		{
			auto const cur = (min + max) / 2;
			auto it = m_allApiSets[cur];

			if (std::starts_with(name, it.first))
				return it.second;

			if (name < it.first)
				max = cur - 1;
			else
				min = cur + 1;
		}
		return {};
	}
};

class V6ApiSetSchema : public ApiSetSchema
{
public:
	std::vector<std::pair<std::wstring, ApiSetTarget>> m_hashedApiSets;

public:
	virtual ApiSetTarget Lookup(std::wstring name)
	{
		// Force lowercase name
		std::transform(name.begin(), name.end(), std::back_inserter(name), std::tolower);

		// Note: The list is initially alphabetically sorted!!!
		size_t min = 0;
		auto max = m_hashedApiSets.size();
		while (min <= max)
		{
			auto const cur = (min + max) / 2;
			auto it = m_hashedApiSets[cur];

			if (std::starts_with(name, it.first))
				return it.second;

			if (name < it.first)
				max = cur - 1;
			else
				min = cur + 1;
		}
		return {};
	}
};

template <typename API_SET_NAMESPACE_VN>
ApiSetSchema* GetApiSetSchemaV2V4(API_SET_NAMESPACE_VN const* const map)
{
	// typedef std::remove_reference_t<decltype(map->Array[0])> array_type;
	auto const base = reinterpret_cast<ULONG_PTR>(map);
	auto const schema = new V2V4ApiSetSchema();
	for (auto it = map->Array, eit = it + map->Count; it < eit; ++it)
	{
		// Retrieve DLLs names implementing the contract
		auto targets = ApiSetTarget();
		auto value_entry = reinterpret_cast<typename API_SET_NAMESPACE_VN::VALUE_ENTRY_TYPE*>(base + it->DataOffset);
		for (auto it2 = value_entry->Redirections, eit2 = it2 + value_entry->NumberOfRedirections; it2 < eit2; ++it2)
		{
			// Retrieve DLLs name implementing the contract
			auto value_buffer = reinterpret_cast<PWCHAR>(base + it2->ValueOffset);
			auto value = std::wstring(value_buffer, 0, it2->ValueLength / sizeof(WCHAR));
			targets.emplace_back(value);
		}

		// Retrieve api min-win contract name
		auto name_buffer = reinterpret_cast<PWCHAR>(base + it->NameOffset);
		auto name = std::wstring(name_buffer, 0, it->NameLength / sizeof(WCHAR));

		// force storing lowercase variant for comparison
		auto const lower_name = std::to_lower(name);

		schema->m_allApiSets.emplace_back(lower_name, targets);
	}
	return schema;
}

ApiSetSchema* GetApiSetSchemaV6(API_SET_NAMESPACE_V6 const* const map)
{
	auto const base = reinterpret_cast<ULONG_PTR>(map);
	auto const schema = new V6ApiSetSchema();
	for (auto it = reinterpret_cast<PAPI_SET_NAMESPACE_ENTRY_V6>(map->EntryOffset + base), eit = it + map->Count; it < eit; ++it)
	{
		// Iterate over all the host dll for this contract
		auto targets = ApiSetTarget();
		for (auto it2 = static_cast<_API_SET_VALUE_ENTRY_V6* const>(reinterpret_cast<PAPI_SET_VALUE_ENTRY_V6>(base + it->ValueOffset)), eit2 = it2 + it->ValueCount; it2 < eit2; ++it2)
		{
			// Retrieve DLLs name implementing the contract
			auto const value_buffer = reinterpret_cast<PWCHAR>(base + it2->ValueOffset);
			auto const value = std::wstring(value_buffer, 0, it2->ValueLength / sizeof(WCHAR));
			targets.emplace_back(value);
		}

		// Retrieve api min-win contract name
		auto const name_buffer = reinterpret_cast<PWCHAR>(base + it->NameOffset);
		auto const name = std::wstring(name_buffer, 0, it->NameLength / sizeof(WCHAR));
		auto const hash_name = std::wstring(name_buffer, 0, it->HashedLength / sizeof(WCHAR));

		// force storing lowercase variant for comparison
		auto lower_name = std::to_lower(name);
		auto lower_hash_name = std::to_lower(hash_name);

		schema->m_allApiSets.push_back({ lower_name, targets });
		schema->m_hashedApiSets.push_back({ lower_hash_name, targets });
	}
	return schema;
}

ApiSetSchema* ParseApiSetSchema(API_SET_NAMESPACE const* const apiSetMap)
{
	// Check the returned api namespace is correct
	if (!apiSetMap)
		return new ApiSetSchema();

	switch (apiSetMap->Version)
	{
	case 2: // Win7
		return GetApiSetSchemaV2V4(&apiSetMap->ApiSetNameSpaceV2);

	case 4: // Win8.1
		return GetApiSetSchemaV2V4(&apiSetMap->ApiSetNameSpaceV4);

	case 6: // Win10
		return GetApiSetSchemaV6(&apiSetMap->ApiSetNameSpaceV6);

	default: // unsupported
		return new ApiSetSchema();
	}
}

void main()
{
	auto ApiSetmapCache = ParseApiSetSchema(GetApiSetNamespace());
	for (auto&& it : ApiSetmapCache->m_allApiSets)
	{
		std::wcout << it.first << L" ---> (";
		for (auto t : it.second)
		{
			std::wcout << t;
			std::wcout << L" ";
		}
		std::wcout << L")" << std::endl;
	}
}
