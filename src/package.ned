package section1;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=359,163");
    submodules:
        Tic: Node {
            @display("p=30,61;i=block/join");
        }
        Toc: Node {
            @display("p=186,61");
        }
    connections:
        Tic.out --> Toc.in;
        Toc.out --> Tic.in;
}

