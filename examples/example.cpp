#include <iostream>
#include <iomanip>

#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
using namespace FIX8;

enum class component : int { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 : int { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };

int main()
{
	/*
	std::cout << get_name(epeek<component, component::user>()) << '\n';
	*/
	if (conjure_enum::is_valid<component, component::password>())
		std::cout << "valid\n";
	else
		std::cout << "not valid\n";
	if (conjure_enum::is_valid<component, static_cast<component>(16)>())
		std::cout << "valid\n";
	else
		std::cout << "not valid\n";

	for(const auto ev : conjure_enum::enum_values<component>)
		std::cout << static_cast<int>(ev) << '\n';
	for(const auto [a, b] : conjure_enum::enum_entries<component>)
		std::cout << static_cast<int>(a) << ' ' << b << '\n';
	for(const auto ev : conjure_enum::enum_values<component1>)
		std::cout << static_cast<int>(ev) << '\n';
	for(const auto ev : conjure_enum::enum_names<component>)
		std::cout << ev << '\n';
	std::cout << conjure_enum::enum_names<component>[5] << '\n';

	std::cout << "1: " << std::string_view(conjure_enum::enum_name_v<component, component::fragment>) << '\n';
	std::cout << "2: " << conjure_enum::enum_name<component, component::fragment>().get() << '\n';
	std::cout << "3: " << conjure_enum::enum_to_string(component::path) << '\n';
	std::cout << "4: " << conjure_enum::enum_to_string(component::test) << '\n';
	std::cout << "5: " << conjure_enum::enum_to_string(path) << '\n';
	std::cout << "6: " << static_cast<int>(conjure_enum::string_to_enum<component>("component::path").value()) << '\n';
	std::cout << "7: " << static_cast<int>(conjure_enum::string_to_enum<component1>("path").value()) << '\n';
	std::cout << "8: " << static_cast<int>(conjure_enum::string_to_enum<component>("wrong").value_or(component(100))) << '\n';
	std::cout << "9: " << conjure_enum::get_name<component1, scheme>() << '\n';
	std::cout << "10: " << conjure_enum::get_name<component, component::scheme>() << '\n';
	std::cout << "16: " << conjure_enum::get_type<component>() << '\n';
	std::cout << "16: " << conjure_enum::get_type<component1>() << '\n';

	//using enum component;
	//std::cout << std::boolalpha << is_scoped<component, scheme>() << '\n';
	std::cout << "11: " << std::boolalpha << conjure_enum::is_scoped<component>() << '\n';
	std::cout << "12: " << std::boolalpha << conjure_enum::is_scoped<component1>() << '\n';
	std::string_view res1 { conjure_enum::enum_name_v<component, component::userinfo> };
	std::cout << "13: " << res1 << '\n';
	std::cout << "14: " << conjure_enum::enum_name_v<component, component::userinfo> << '\n';
	std::cout << "15: " << conjure_enum::count<component>() << '\n';
	if (auto result {conjure_enum::int_to_enum<component>(100)}; result)
		std::cout << static_cast<int>(*result) << '\n';
	else
		std::cout << "invalid int to enum\n";
	if (auto result {conjure_enum::int_to_enum<component>(12)}; result)
		std::cout << static_cast<int>(*result) << '\n';
	else
		std::cout << "invalid int to enum\n";

	return 0;
}
