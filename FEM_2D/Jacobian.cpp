#include "stdafx.h"
#include "Jacobian.h"




Jacobian::Jacobian()
{
	element_universal = new Element_Universal();
	//GlobalData::printArray(element_universal->getDn_dEta(), element_universal->get_matrix_size(), element_universal->get_matrix_size(), "dN_dEta");
	//GlobalData::printArray(element_universal->getDn_dKsi(), element_universal->get_matrix_size(), element_universal->get_matrix_size(), "dN_dKsi");
	//GlobalData::printArray(element_universal->getN(), element_universal->get_matrix_size(), element_universal->get_matrix_size(), "N");
	

}

Element_Universal* Jacobian::getElement_universal()
{
	return element_universal;
}
int Jacobian::get_jacobian_size()
{
	return jacobian_size;
}

int Jacobian::get_matrix_size()
{
	return matrix_size;
}

double* Jacobian::get_dX_dEta()
{
	return dX_dEta;
}

double* Jacobian::get_dX_dKsi()
{
	return dX_dKsi;
}

double* Jacobian::get_dY_dEta()
{
	return dY_dEta;
}

double* Jacobian::get_dY_dKsi()
{
	return dY_dKsi;
}

double* Jacobian::get_det_J()
{
	return this->det_J;
}

double** Jacobian::getdN_dX()
{
	return this->dN_dX;
}

double** Jacobian::getdN_dY()
{
	return this->dN_dY;
}

void Jacobian::initDN_Matrixes()
{
	dN_dX = new double*[numberOfLocalJacobians];
	dN_dY = new double*[numberOfLocalJacobians];

	for (int i = 0; i < numberOfLocalJacobians; i++)
	{
		dN_dX[i] = new double[numberOfLocalJacobians];
		dN_dY[i] = new double[numberOfLocalJacobians];
	}

	for (int j = 0; j < numberOfLocalJacobians; j++) {
		for (int i = 0; i < numberOfLocalJacobians; i++)
		{
			// TODO check matrix Dn_DEta indexes!
			dN_dX[i][j] = rev_jacobian[0][0][i] * element_universal->dN_dKsi[i][j] - rev_jacobian[0][1][i] * element_universal->dN_dEta[i][j];
			dN_dY[i][j] = rev_jacobian[1][0][i] * element_universal->dN_dKsi[i][j] + rev_jacobian[1][1][i] * element_universal->dN_dEta[i][j];
		}
	}
}

void Jacobian::calculate_Jacobians()
{
	jacobian = new double**[jacobian_size];
	rev_jacobian = new double**[jacobian_size];
	det_J = new double[numberOfLocalJacobians];

	for (int i = 0; i < jacobian_size; i++)
	{
		jacobian[i] = new double*[jacobian_size];
		rev_jacobian[i] = new double*[jacobian_size];
		for (int j = 0; j < jacobian_size; j++) {
			jacobian[i][j] = new double[numberOfLocalJacobians];
			rev_jacobian[i][j] = new double[numberOfLocalJacobians];
			for (int k = 0; k < numberOfLocalJacobians; k++) {
				jacobian[i][j][k] = 0;
				rev_jacobian[i][j][k] = 0;

			}
		}
	}

	for (int i = 0; i < numberOfLocalJacobians; i++)
	{
		jacobian[0][0][i] = dX_dKsi[i];
		jacobian[0][1][i] = dY_dKsi[i];
		jacobian[1][0][i] = dX_dEta[i];
		jacobian[1][1][i] = dY_dEta[i];
	}


	for (int i = 0; i < numberOfLocalJacobians; i++)
	{
		det_J[i] = jacobian[0][0][i] * jacobian[1][1][i] - jacobian[0][1][i] * jacobian[1][0][i];
	}

	for (int i = 0; i < numberOfLocalJacobians; i++)
	{
		rev_jacobian[0][0][i] = jacobian[1][1][i] / det_J[i];
		rev_jacobian[0][1][i] = (-1)*jacobian[0][1][i] / det_J[i];
		rev_jacobian[1][0][i] = (-1)*jacobian[1][0][i] / det_J[i];
		rev_jacobian[1][1][i] = jacobian[0][0][i] / det_J[i];

	}

	/*
	jacobian[0][0] = dX_dKsi[0];
	jacobian[0][1] = dY_dKsi[0];
	jacobian[1][0] = dX_dEta[0];
	jacobian[1][1] = dY_dEta[0]; */
}



void Jacobian::print_jacobian()
{
	printf("\n**********************\n");
	for (int i = 0; i < numberOfLocalJacobians; i++)
	{
		printf("Jacobian %i\n", i);
		for (int j = 0; j < jacobian_size; j++)
		{
			for (int k = 0; k < jacobian_size; k++)
			{
				printf("  %.8f ", jacobian[j][k][i]);
			}
			printf("\n");
		}
		printf("\n");

		printf("Reversed Jacobian %i\n", i);
		for (int j = 0; j < jacobian_size; j++)
		{
			for (int k = 0; k < jacobian_size; k++)
			{
				printf("  %.8f ", rev_jacobian[j][k][i]);
			}
			printf("\n");
		}
		printf("\n");
		
		printf("Det J %i\n", i);
		printf("  %.8f\n", det_J[i]);

		printf("**********************\n");

	}
}

void Jacobian::initDy_DEta(std::vector<double> y)
{
	dY_dEta = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dY_dEta[i] = 0;
	}
	for (int i = 0; i < matrix_size; i++)
	{
		// TODO -> Uwaga tutaj wprowadzam zmiany wieksze

		dY_dEta[i] += element_universal->getDn_dEta()[0][i] * y[0];
		dY_dEta[i] += element_universal->getDn_dEta()[1][i] * y[1];
		dY_dEta[i] += element_universal->getDn_dEta()[2][i] * y[2];
		dY_dEta[i] += element_universal->getDn_dEta()[3][i] * y[3];  
		/*for (int m = 0; m < matrix_size; ++m) {
			dY_dEta[i] += element_universal->getDn_dEta()[0][m] * y[i];
			dY_dEta[i] += element_universal->getDn_dEta()[1][m] * y[i];
			dY_dEta[i] += element_universal->getDn_dEta()[2][m] * y[i];
			dY_dEta[i] += element_universal->getDn_dEta()[3][m] * y[i];
		}  */


	}
}

void Jacobian::initDy_DKsi(std::vector<double> y)
{
	dY_dKsi = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dY_dKsi[i] = 0;
	}
	for (int i = 0; i < matrix_size; i++)
	{

		dY_dKsi[i] += element_universal->getDn_dKsi()[0][i] * y[0];
		dY_dKsi[i] += element_universal->getDn_dKsi()[1][i] * y[1];
		dY_dKsi[i] += element_universal->getDn_dKsi()[2][i] * y[2];
		dY_dKsi[i] += element_universal->getDn_dKsi()[3][i] * y[3];
	}
}

void Jacobian::initDx_DEta(std::vector<double> x) {
	dX_dEta = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dEta[i] = 0;
	}
	for (int i = 0; i < matrix_size; i++)
	{

		dX_dEta[i] += element_universal->getDn_dEta()[0][i] * x[0];
		dX_dEta[i] += element_universal->getDn_dEta()[1][i] * x[1];
		dX_dEta[i] += element_universal->getDn_dEta()[2][i] * x[2];
		dX_dEta[i] += element_universal->getDn_dEta()[3][i] * x[3];

	}
}

void Jacobian::initDx_DKsi(std::vector<double> x)
{
	dX_dKsi = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dKsi[i] = 0;
	}
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dKsi[i] += element_universal->getDn_dKsi()[0][i] * x[0];
		dX_dKsi[i] += element_universal->getDn_dKsi()[1][i] * x[1];
		dX_dKsi[i] += element_universal->getDn_dKsi()[2][i] * x[2];
		dX_dKsi[i] += element_universal->getDn_dKsi()[3][i] * x[3];

	}
}
/* ******************************************************************** */
/*
void Jacobian::initDy_DEta(double y1, double y2, double y3, double y4)
{
	dY_dEta = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dY_dEta[i] = 0;
	}
	double y[4] = { y1,y2,y3,y4 };
	for (int i = 0; i < matrix_size; i++)
	{

		dY_dEta[i] += element_universal->getDn_dEta()[0][i] * y[0];
		dY_dEta[i] += element_universal->getDn_dEta()[1][i] * y[1];
		dY_dEta[i] += element_universal->getDn_dEta()[2][i] * y[2];
		dY_dEta[i] += element_universal->getDn_dEta()[3][i] * y[3];

	}
}

void Jacobian::initDy_DKsi(double y1, double y2, double y3, double y4)
{
	dY_dKsi = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dY_dKsi[i] = 0;
	}
	double y[4] = { y1,y2,y3,y4 };
	for (int i = 0; i < matrix_size; i++)
	{

		dY_dKsi[i] += element_universal->getDn_dKsi()[0][i] * y[0];
		dY_dKsi[i] += element_universal->getDn_dKsi()[1][i] * y[1];
		dY_dKsi[i] += element_universal->getDn_dKsi()[2][i] * y[2];
		dY_dKsi[i] += element_universal->getDn_dKsi()[3][i] * y[3];
	}
}

void Jacobian::initDx_DEta(double x1, double x2, double x3, double x4) {
	dX_dEta = new double[matrix_size];
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dEta[i] = 0;
	}
	double x[4] = { x1,x2,x3,x4 };
	for (int i = 0; i < matrix_size; i++)
	{

		dX_dEta[i] += element_universal->getDn_dEta()[0][i] * x[0];
		dX_dEta[i] += element_universal->getDn_dEta()[1][i] * x[1];
		dX_dEta[i] += element_universal->getDn_dEta()[2][i] * x[2];
		dX_dEta[i] += element_universal->getDn_dEta()[3][i] * x[3];

	}
}

void Jacobian::initDx_DKsi(double x1, double x2, double x3,double x4)
{
	dX_dKsi = new double[matrix_size];
	double x[4] = { x1,x2,x3,x4 };
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dKsi[i] = 0;
	}
	for (int i = 0; i < matrix_size; i++)
	{
		dX_dKsi[i] += element_universal->getDn_dKsi()[0][i] * x[0];
		dX_dKsi[i] += element_universal->getDn_dKsi()[1][i] * x[1];
		dX_dKsi[i] += element_universal->getDn_dKsi()[2][i] * x[2];
		dX_dKsi[i] += element_universal->getDn_dKsi()[3][i] * x[3];

	}
}
*/

Jacobian::~Jacobian()
{
}
