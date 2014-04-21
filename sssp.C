#include <stdio.h>
#include <vector>
#include <limits>
#include "sssp.decl.h"

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ int nScale = 4;
/*readonly*/ int nK = 4;

/*mainchare*/
class Main : public CBase_Main
{
public:
  Main(CkArgMsg* m)
  {
    //Process command-line arguments
		// TODO: add command-line parsing

    CkPrintf("Running SSSP on %d processors for graph of scale %d \n",
	     CkNumPes(), nScale);
    mainProxy = thisProxy;

		// create graph
    CProxy_SSSPVertex g = CProxy_SSSPVertex::ckNew(1<<nScale);
		for (int i = 0; i < (1<<nScale); i++) 
			g[i].init(g);
	
		// run SSSP computation
		//g[0].make_root();
		do_sssp(g,0);
 
		// I dont know how to stop computation otherwise
		CkStartQD(CkIndex_Main::done(), &thishandle);
  };

	void do_sssp(CProxy_SSSPVertex &g, int root) 
	{
		  CkPrintf("root <-- %d\n",root); 
			g[root].compute(0);
	}

	void done_sssp() 
	{
	}

  void done(void)
  {
    CkPrintf("All done\n");
    CkExit();
  };
};

class SSSPVertex : public CBase_SSSPVertex 
{
private:
	std::vector<long long> adjlist;
	std::vector<double> edge_weights;
	double dist;

	CProxy_SSSPVertex g;

public:
	SSSPVertex() 
	{
    //CkPrintf("SSSPVertex %d created\n",thisIndex);
		// initialize adjacency list
		for (int i = 0; i < nK; i++) {
			adjlist.push_back(rand() % (1<<nScale));
			edge_weights.push_back(drand48());
		}

		dist = std::numeric_limits<double>::max();
	}
	void init(CProxy_SSSPVertex &g) { this->g = g; }
	void make_root() 
	{
		CkPrintf("SSSPVertex %d: root\n",thisIndex);
    dist = 0;
	}
	void print() 
	{
    CkPrintf("SSSPVertex %d: dist = %f\n",thisIndex,dist);
	}
  SSSPVertex(CkMigrateMessage *m) {}

	void compute(double sum) {
		if (sum < dist) {
			dist = sum;

			CkPrintf("SSSPVertex %d: dist updated to %f\n",thisIndex, dist);
			for (int i = 0; i < adjlist.size(); i++) {
				g[adjlist[i]].compute(dist + edge_weights[i]);
			}
		}
	}
};

#include "sssp.def.h"
