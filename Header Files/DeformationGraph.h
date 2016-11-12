#ifndef __DEFORMATION_GRAPH_H__
#define __DEFORMATION_GRAPH_H__

#ifndef __OPENCV_HPP__
#define __OPENCV_HPP__
#include <opencv2/opencv.hpp>
using namespace cv;
#endif

typedef double Node[3];
typedef double Rotation[9];
typedef double Translation[3];


class DeformationGraph{
public:
	DeformationGraph();
	//input nodes and edges data to create a deformation graph
	//the next version may create a deformation graph according to mesh data
	DeformationGraph(int n_nodes, Node *nodes, int n_edges, bool **edges);
	
	~DeformationGraph();

	//calculate the weight of a vertex
	//weights is a double array, contains k-nearest nodes' weights
	//idx is an int array, contain k+1-nearest nodes' indexes
	void computeWeights(const double *vertex, double *weights, int *idx);

	//predict the position of a vertex or a node
	//if the number of nodes of deformation graph is 0, the output will be (0,0,0)
	void predict(const double *v, double *_v);

	double Erot();  //compute the rotation error
	double Ereg();  //compute the regularization error
	//compute the regularization error of the j-th node and the n-th node
	//return (Ereg(j,n))^(1/2)
	//vector e = Rj(gn-gj)+gj+tj-(gn+tn)
	double Ereg(const int j, const int n, double *e);
	double Econ(const int p, double **v, double **q);  //compute the constraints error
	//double Econ(const int p, const double **v, const double **q, const int *idx);

	//compute the value of the objective function
	/*inline double F(const double w_rot, const double w_reg, const double w_con){
		return w_rot*e_rot+w_reg*e_reg+w_con*e_con;
	}*/

	friend void computef(DeformationGraph &dg, int &p, double **v, double **q, OutputArray f);
	friend void computeJ(DeformationGraph &dg, const int &p, double **v, double **q, OutputArray J);
	//x is used to update dg
	friend double F(DeformationGraph &dg, Mat &x, int p, double **v, double **q);
	friend void gaussNewton(DeformationGraph &dg, int p, double **v, double **q);

private:
	int n_nodes;   //number of nodes
	int n_edges;   //number of edges
	int k_nearest; //number of k (k-nearest nodes)
	Node *nodes;   //nodes array
	bool **edges;  //edges, a adjacency matrix, n_nodes * n_nodes dimensions
	Rotation *rot; //rotation matrixes
	Translation *trans;   //translation vectors
	double e_rot;   //rotation error
	double e_reg;   //regularization error
	double e_con;   //constraints error
	double w_rot;   //rotation error weight
	double w_reg;   //regularization error weight
	double w_con;   //constraints error weig
};

#endif