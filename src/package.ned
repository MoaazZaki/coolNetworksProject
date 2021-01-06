package coolNetworks;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    parameters:
        int numberofNodes = default(6);
		//volatile double RESTART @unit(s) = default(7.0s);
        @display("bgb=359,163");
        *.numberofNodes = numberofNodes;
    submodules:
        parent: Parent {
            //n=numberofNodes;
            @display("p=242.15001,52.605");
        }

        nodes[numberofNodes]: Node {
			//n=numberofNodes;
            @display("p=80.995,60.955");
        }

    connections:
        //connecting all nodes with each other to form distibuted network architecture
        for i=0..numberofNodes-1, for j=i+1..numberofNodes-1 {
            nodes[i].out++ --> nodes[j].in++;
            nodes[i].in++ <-- nodes[j].out++;
        }

        //connecting parent to all nodes
        for i=0..numberofNodes-1 {
            parent.out++ --> nodes[i].in++;
            parent.in++ <-- nodes[i].out++;
        }

}

