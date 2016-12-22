#include<iostream>
#include"Region.h"
#include<vector>

#if defined (__cplusplus)
extern "C"{
#endif
	#include<stdio.h>
	#include"luaT.h"
	#include"TH.h"
	int luaopen_Linterface(lua_State*);
#if defined (__cplusplus)
}
#endif

class MPGraphInst{//singleton class
private:
	static MPGraphInst *inst;
	MPGraphInst(){}
	MPGraphInst(MPGraphInst&){//Empty copy constructor
				 }
public:	static MPGraphInst* instance(){
		if(!inst)
			inst = new MPGraphInst;
		return inst;

	}
	MPGraph<double,int> g;
	
        std::vector<MPGraph<double,int>::PotentialID> pots;
        std::vector<MPGraph<double,int>::RegionID> regs;
};

MPGraphInst* MPGraphInst::inst=NULL;//instantiating static pointer inst

class validator{ //to make sure the operations/function calls are done in the
//right order
	bool addVar;
	bool addPot;
	bool addReg;
public:
	validator(){
		addVar=false;
		addPot=false;
		addReg=false;
	}
	bool get_addVar(){
		return addVar;
	}
	bool get_addPot(){
		return addPot;
	}
	bool get_addReg(){
		return addReg;
	}
	void set_addVar(){
		addVar=true;
	}
	void set_addPot(){
		addPot=true;
	}
	void set_addReg(){
		addReg=true;
	}
};

validator validate;

//-----------------ADD REGION FUNCTION-----------------//

static int addVar(lua_State *L){
	
	if(!lua_isnumber(L,1)|| !lua_istable(L,2)){
		printf("\nImproper arguments to \'addVariables\'\n");
		return 0;
	}

	int no_of_var = lua_tonumber(L,1);
	std::vector<int> inputVars(no_of_var,0);
	for(int i=0;i<no_of_var;i++){
		lua_pushnumber(L,i+1);
		lua_gettable(L,2);
		inputVars[i]=lua_tonumber(L,-1);
		lua_pop(L,1);
	}
	MPGraphInst::instance()->g.AddVariables(inputVars);
	validate.set_addVar();
	
return 0;
}

//----------------SET POTENTIALS FUNCTION--------------//

static int addPot(lua_State *L){
	
	if(!validate.get_addVar()){
		printf("\nInput Variable Parameters first\n");
		return 0;
	}

	if (!lua_isnumber(L,1) || !lua_istable(L,2)){
		printf("\nImproper arguments to \'addVariables\'\n");
		return 0;
	}
	//extracting no of potentials in the table
	int no_dataPots = lua_tonumber(L,1);

	double *dataPots = (double*)malloc(sizeof(double)*no_dataPots);		
	
	//extracting table of potentials
	for(int i=0;i<no_dataPots;i++){
                lua_pushnumber(L,i+1);
                lua_gettable(L,2);
                dataPots[i]=lua_tonumber(L,-1);
                lua_pop(L,1);
        }

	MPGraphInst::instance()->pots.push_back(MPGraphInst::instance()->g.AddPotential(MPGraph<double,int>::PotentialVector(dataPots,no_dataPots)));

	//remember to deallocate dataPots (where?)
	//note:arrays are passed by reference. Don't remove until not needed

	validate.set_addPot();
return 0;
}

//---------------------ADD REGION FUNCTION-------------------//

static int addRegion(lua_State *L){
	if(!validate.get_addPot()){
                  printf("\nInput Potentials first\n");
                return 0;
        }

	if(!lua_isnumber(L,1) ||!lua_isnumber(L,2) || !lua_istable(L,3) || !lua_isnumber(L,4)){
		printf("\nImproper arguments to \'addRegion\'\n");
                return 0;
	}

	double arg1 = lua_tonumber(L,1);
	int noOfVar = lua_tonumber(L,2);
	std::vector<int> reg(noOfVar,0);
	int potType = lua_tonumber(L,4);

	for(int i=0;i<noOfVar;i++){
		lua_pushnumber(L,i+1);
		lua_gettable(L,3);
		reg[i]=lua_tonumber(L,-1);
		lua_pop(L,1);
	}

	 MPGraphInst::instance()->regs.push_back(MPGraphInst::instance()->g.AddRegion(arg1, reg, MPGraphInst::instance()->pots[potType]));
	
	validate.set_addReg();
	
return 0;
}

static int getBeliefs(lua_State *L){

	if(!validate.get_addReg()){
		  printf("\nInput Regions first\n");
                return 0;
	}
	MPGraphInst::instance()-> g.AllocateMessageMemory();

        double epsilon = 0.0;

        CPrecisionTimer CTmr;
        CTmr.Start();
        RMP<double, int> RMPAlgo;
        RMPAlgo.RunMP(MPGraphInst::instance()->g, epsilon, 10);
        std::cout << CTmr.Stop() << std::endl;

        double* beliefs;
        size_t beliefSize=RMPAlgo.GetBeliefs(MPGraphInst::instance()->g, epsilon, &beliefs, true);
	
	THDoubleStorage *store = THDoubleStorage_newWithData(beliefs,beliefSize);

	THDoubleTensor *newTens = THDoubleTensor_newWithStorage1d(store,0,beliefSize,1);

	
	luaT_pushudata(L,(void*)newTens,"torch.DoubleTensor");

	//beliefs will be deallocated by Lua's garbage collector


return 1;
}


int luaopen_Linterface(lua_State *L){
	lua_register(L,"addVariables",addVar);
	lua_register(L,"addPotential",addPot);
	lua_register(L,"addRegion",addRegion);
	lua_register(L,"getBeliefs",getBeliefs);
return 0;
}
