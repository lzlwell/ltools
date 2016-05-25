#include"kplusq.h"


// This program reads k-point, and q-point, and generate a k+q file
// with the same points removed
// This can be used to linearly increase computation time in sigma
// without recalculating epsilon
// Note that eps0mat doesn't need to be considered even with subsampling
// because in sigma they use the same wfn at the same k point
// Setting wfn_weight to 1 for all points

// All points dealt with here are fractional points

#define tol_small 0.000001

double dist(double *k1, double *k2)
{
  int i;
  double disqr = 0.0;

  for(i=0; i<3; i++)
    disqr += (k1[i] - k2[i]) * (k1[i] - k2[i]);

  return sqrt(disqr);
}


void map_uc(double *kpt)
{
  int i;

  for(i=0; i<3; i++)
  {
    while(kpt[i] >= 1.0)
      kpt[i] --;
    while(kpt[i] < 0.0)
      kpt[i] ++;
  }

  return;
}


int main()
{
  int nk, nq, nkpq, nuse;
  double **kpts, **qpts, **kpqpts, **kpquse;
  int i, j; 
  int ik, iq, ikpq, iuse;
  int ifnew;

  FILE *fk, *fq, *fkpq;

  fk = fopen("kpoints", "r");
  fq = fopen("qpoints", "r");
  fkpq = fopen("kpqpoints", "w");

  while(fgetc(fk) != '\n');
  fscanf(fk, "%d", &nk);
  while(fgetc(fk) != '\n');

  while(fgetc(fq) != '\n');
  fscanf(fq, "%d", &nq);
  while(fgetc(fq) != '\n');

  nkpq = nk*nq;
  nuse = 0;

  kpts = (double**)malloc(nk * sizeof(double*));
  qpts = (double**)malloc(nq * sizeof(double*));
  kpqpts = (double**)malloc(nkpq * sizeof(double*));
  kpquse = (double**)malloc(nkpq * sizeof(double*));

  for(ik=0; ik<nk; ik++)
    kpts[ik] = (double*)malloc(3 * sizeof(double));
  for(iq=0; iq<nq; iq++)
    qpts[iq] = (double*)malloc(3 * sizeof(double));
  for(ikpq = 0; ikpq < nkpq; ikpq++)
  {
    kpqpts[ikpq] = (double*)malloc(3 * sizeof(double));
    kpquse[ikpq] = (double*)malloc(3 * sizeof(double));
  }


  for(ik=0; ik<nk; ik++)
  {
    fscanf(fk, "%lf %lf %lf", &kpts[ik][0], &kpts[ik][1], &kpts[ik][2]);
    while(fgetc(fk) != '\n');
  }

  for(iq=0; iq<nq; iq++)
  {
    fscanf(fq, "%lf %lf %lf", &qpts[iq][0], &qpts[iq][1], &qpts[iq][2]);
    while(fgetc(fq) != '\n');
  }

// generating all kpq points
  ikpq = -1;
  for(ik=0; ik<nk; ik++)
  {
    for(iq=0; iq<nq; iq++)
    {
      ikpq ++;
      for(i=0; i<3; i++)
      {
        kpqpts[ikpq][i] = kpts[ik][i] + qpts[iq][i];
      }
    }
  }

// map all kpq points back to unit cell
  for(ikpq = 0; ikpq < nkpq; ikpq++)
    map_uc(kpqpts[ikpq]);


  nuse = 1;
  for(i=0; i<3; i++)
    kpquse[nuse-1][i] = kpqpts[0][i];

  for(ikpq=0; ikpq<nkpq; ikpq++)
  {
    ifnew = 1;

    for(iuse=0; iuse<nuse; iuse++)
      if( dist(kpqpts[ikpq], kpquse[iuse]) < tol_small )
        ifnew = 0;

    if(ifnew == 1)
    {
      nuse ++;
      for(i=0; i<3; i++)
        kpquse[nuse-1][i] = kpqpts[ikpq][i];
    }
  }


  fprintf(fkpq, "K_POINTS crystal\n   %d  \n", nuse);

  for(iuse=0; iuse<nuse; iuse++)
    fprintf(fkpq, "      %f   %f   %f     1.0\n", kpquse[iuse][0], kpquse[iuse][1], kpquse[iuse][2]);


  fclose(fk);
  fclose(fq);
  fclose(fkpq);


  return 0;

}


