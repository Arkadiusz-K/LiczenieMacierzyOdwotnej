#include "matrix.h"
void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim ); 
void ComplMatrix( double** pTabD, double** pTab, int nDim );
void TransMatrix( double** pTab, int nDim );


int CreateMatrix( double*** pTab, int nSize )
{
	*pTab = ( double** )malloc( nSize * sizeof( double* ) );
	if ( !pTab )
		return 0;
	double **pV = *pTab;
	for ( int i = 0; i < nSize; i++ )
	{
		*pV = ( double* )malloc( nSize * sizeof( double ) );
		if ( !*pV )
			return 0;
		memset( *pV++, 0, nSize * sizeof( double ) );
	}
	return 1;
}



void DeleteMatrix( double*** pTab, int nSize )
{
	double **pPom = *pTab;
	for ( int i = 0; i < nSize; i++ )
		free( *pPom++ );
	free( *pTab );
}



void InverseMatrix( double** pInv, double **pTab, int nSize, double det )
{
	ComplMatrix( pInv, pTab, nSize );	// macierz dope³nieñ
	TransMatrix( pInv, nSize );	// Obróc
	for ( int i = 0; i < nSize; i++ )
	{
		double *pV = *pInv++;
		for ( int j = 0; j < nSize; j++ )
			*pV++ /= det;	// Podziel przez wyznacznik
	}
}



double Det( double** pTab, int nSize )
{
	if ( nSize == 1 )	// Wymiar 1 -> zwrocic wynik
		return **pTab;
	if ( nSize == 2 )	// Wymiar 2 -> wedlug wzoru
	{
		return **pTab * pTab[ 1 ][ 1 ] - pTab[ 0 ][ 1 ] * pTab[ 1 ][ 0 ];
	}
	else
	{
		double res = 0;
		double **pPomTab = NULL;
		if ( !CreateMatrix( &pPomTab, nSize - 1 ) )	// Macierz o 1 wymiar mniejsza
		{
			perror( "Blad alokacji pamieci! Det" );
			return 1;
		}
		double *pV = *pTab;
		double znak = 1;
		for ( int i = 0; i < nSize; i++ )
		{
			Complement( pPomTab, pTab, 0, i, nSize );		// Wykreslenie zerowego wiersza i odpowiedniej kolumny
			res += znak * *pV++ * Det( pPomTab, nSize - 1 ); // wyznacznik macierzy powstalej po wykresleniu*wykreslany element*znak
			znak = -znak;									// Znak na zmiane
		}
		DeleteMatrix( &pPomTab, nSize - 1 );				// usuniecie macierzy pomocniczej
		return res;
		}
	}





void LayoutEqu( double** pInv, double* pB, double* pRes, int nSize )
{
	for ( int i = 0; i < nSize; i++ )
	{
		double *pInvIt = *pInv++;
		double *pBIt = pB;
		for ( int j = 0; j < nSize; j++ )
			*pRes += *pInvIt++ * *pBIt++;
		pRes++;
	}
}




void PrintMatrix( double** pTab, int nSize )
{
	for ( int i = 0; i < nSize; i++ )
	{
		double* pPom = *pTab++;
		for ( int j = 0; j < nSize; j++ )
			printf( "%lf ", *pPom++ );
		printf( "\n" );
	}
	printf( "\n" );
}



void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim )
{
	for ( int i = 0; i < nDim; i++ )
	{
		if ( i == nRow )	// Pominiecie wiersza gdzie "wykreslany" element
		{
			pTabI++;
			continue;
		}

		double *pI = *pTabI++;
		double *pO = *pTabO++;
		for ( int j = 0; j < nDim; j++ )
		{
			if ( j == nCol )	// Pominicie kolumny gdzie wykreslany element
			{
				pI++;
				continue;
			}
			*pO++ = *pI++;
		}

	}
}



void ComplMatrix( double** pTabD, double** pTab, int nDim )
{
	double **pPomTab = NULL;
	if ( !CreateMatrix( &pPomTab, nDim - 1 ) )
	{
		perror( "Blad alokacji pamieci! CompMatrix" );
		return;
	}
	double **pTabCopy = pTab;
	for ( int i = 0; i < nDim; i++ )
	{
		double *pI = *pTabCopy++;
		double *pO = *pTabD++;
		double znak = i % 2 ? -1 : 1;	// Znak zaleznie od tego gdzie sie znajduje element
		for ( int j = 0; j < nDim; j++ )
		{
			Complement( pPomTab, pTab, i, j, nDim );	// "Wykreslenie" danego wiersza i kolumny
			*pO++ = znak * Det( pPomTab, nDim - 1 );	// wyznacznik macierzy po wykresleniu elementu * znak
			znak = -znak;								// Znak na zmiane
		}
	}
	DeleteMatrix( &pPomTab, nDim - 1 );
}


void TransMatrix(double** pTab, int nDim)
{
	double ** p = pTab;
	for (int i = 0; i < nDim - 1; i++)
	{
		double* v = *p + i + 1;
		for (int j = i + 1; j < nDim; j++)
		{
			double tmp = *v;
			*v++ = pTab[j][i];
			pTab[j][i] = tmp;
		}
	}
}