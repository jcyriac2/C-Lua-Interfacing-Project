require("Linterface")

addVariables(3,{2,2,2})

addPotential(2,{0.0,1.0})
addPotential(4,{1.0,0.0,0.0,1.0})

for i=0,2 do
	addRegion(1.0,1,{i},0)
end

addRegion(1.0,2,{0,1},1)
addRegion(1.0,2,{1,2},1)
addRegion(1.0,2,{0,2},1)

a=getBeliefs()
print("Beliefs in 1D Tensor:")
print(a)
