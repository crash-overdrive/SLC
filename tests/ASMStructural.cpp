#include "catch.hpp"
#include <iostream>
#include "ASMServices.hpp"
#include "ASMStructuralLib.hpp"


TEST_CASE("Code Generation Libraries", "[ASM-CodeGen]") {
	ASM::AnonymousLabelService ALSClient{};
	ASM::StructuralLib slib{ALSClient};
	
	SECTION("Unique Labels are unique") {
		REQUIRE(ALSClient.getUniqueId() != ALSClient.getUniqueId());
		REQUIRE(ALSClient.getUniqueLabel() != ALSClient.getUniqueLabel());
	}

	SECTION("IfElseCodeIsSensible") {
		//pretend the condition was the first argument
		std::string condition = "MOV EAX, [EBP + 12];\n";

		std::string s1 = "MOV [EBP - 4], 2;\n";
		std::string s2 = "MOV [EBP - 4], 1;\n";

		std::string output = slib.IfThenElse(condition, s1, s2);

		std::cout<<"IfElse"<<std::endl;
		std::cout<<output<<std::endl;

		//hard to test, as labels are based on side effects. Will investigate
		//REQUIRE(1==1);
	}


	SECTION("While Loop") {
		std::string condition = "MOV EAX. [EBP - 4]\n";
		std::string statement = "MOV EAX, [EBP - 4];\nADD EAX, 4;\nMOV [EBP - 4], EAX;\n";
		std::string output = slib.WhileLoop(condition, statement);
		std::cout<<"While Loop"<<std::endl;
		std::cout<<output<<std::endl;
	}

	SECTION("For Loop"){

	}
}
