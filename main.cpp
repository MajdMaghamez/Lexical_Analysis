#include<String>
#include<iostream>
using namespace std;

int main (){


	return 0;
}

bool CompareStrings (string str1, string str2){
	int points = 0;
	for(int i = 0; i < str1.length(); i++){
		if(str1[i] == str2[i]){
			points++;
		}
		else{
			points--;
		}
	}
	if(points == str1.length()){
		return true;
	}
	else{
		return false;
	}
}

bool Valid_Keywords(string str){
    string ListOfIdentifiers [] = {"__abstract", "__alignof Operator", "__asm", "__assume", "__based", "__box",
    "__cdecl","__declspec", "__delegate", "__event", "__except", "__fastcall", "__finally", "__forceinline",
    "__gc", "__hook","__identifier", "__if_exists", "__if_not_exists", "__inline", "__int16", "__int32",
    "__int64", "__int8","__interface", "__leave", "__m128", "__m128d", "__m128i", "__m64",
    "__multiple_inheritance", "__nogc", "__noop","__pin", "__property", "__raise", "__sealed",
    "__single_inheritance", "__stdcall", "__super", "__thiscall","__try", "__try_cast", "__unaligned",
    "__unhook", "__uuidof", "__value", "__virtual_inheritance", "__w64", "__wchar_t","wchar_t", "abstract",
    "array", "auto", "bool", "break", "case", "catch", "char", "class", "const", "const_cast","continue",
    "decitype", "default", "delegat", "delete", "deprecated", "dllexport", "dillimport", "do", "double",
    "dynamic_cast", "else", "enum", "enum class", "enum struct", "event", "explicit", "extern", "false",
    "finally","float", "for", "for each", "in", "friend", "friend_as", "gcnew", "generic", "goto", "if",
    "initonly", "inline","int", "interface class", "interface struct", "interior_ptr", "literal", "long",
    "mutable", "naked", "namespace","new", "noinline", "noreturn", "nothrow", "novtable", "nullptr", "operator",
    "private", "property", "protected","public", "ref class", "ref struct", "register", "reinterpret_cast",
    "return", "safecast", "sealed", "selectany","short", "signed", "sizeof", "static", "static_assert",
    "static_cast", "struct", "switch", "template", "this","tread", "throw", "true", "try", "typedef", "typeid",
    "typename", "union", "unsigned", "using declaration","using directive", "uuid", "value class", "value struct",
    "virtual", "void", "volatile", "while"};

    for(int i = 0; i < 150; i++){
    	if(CompareStrings(str, ListOfIdentifiers[i])){
		return true;
		}
	}
	return false;
}
