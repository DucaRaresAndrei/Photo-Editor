//Duca Andrei-Rares
//311CA
#include <functions.h>

int **alloc_m(int m, int n)
{
	int **a = malloc(m * sizeof(int *));

	if (!a)
		return NULL;

	for (int i = 0; i < m; i++) {
		a[i] = calloc(n, sizeof(int));

		if (!a) {
			for (int j = i; j >= 0; j--)
				free(a[j]);

			free(a);

			return NULL;
		}
	}

	return a;
}

void free_m(int **mat, int h)
{
	if (mat) {
		for (int i = 0; i < h; i++)
			free(mat[i]);

		free(mat);
	}
}

void skip_comm(FILE *in)
{
	char a;
	fpos_t poz;

	fgetpos(in, &poz);
	fscanf(in, "%c", &a);

	while (a == '#') {
		while (a != '\n')
			fscanf(in, "%c", &a);

		fgetpos(in, &poz);

		fscanf(in, "%c", &a);
	}

	//the cursor is positioned at the end of the lines with comments
	fsetpos(in, &poz);
}

void p2(FILE *in, int **gr, int h, int l)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < l; j++)
			fscanf(in, "%d", &gr[i][j]);
}

void p5(FILE *in, int **gr, int h, int l)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < l; j++)
			fread(&gr[i][j], sizeof(char), 1, in);
}

void p3(FILE *in, int **r, int **g, int **b, int h, int l)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < l; j++) {
			fscanf(in, "%d", &r[i][j]);
			fscanf(in, "%d", &g[i][j]);
			fscanf(in, "%d", &b[i][j]);
		}
}

void p6(FILE *in, int **r, int **g, int **b, int h, int l)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < l; j++) {
			fread(&r[i][j], sizeof(char), 1, in);
			fread(&g[i][j], sizeof(char), 1, in);
			fread(&b[i][j], sizeof(char), 1, in);
		}
}

void free_all(struct colors *st)
{
	if ((*st).ok == 2 || (*st).ok == 5) {
		free_m((*st).gr, (*st).h);
	} else {
		free_m((*st).r, (*st).h);
		free_m((*st).g, (*st).h);
		free_m((*st).b, (*st).h);
	}
}

void load(struct colors *st, char *zone)
{
	char filename[256], a;
	int chc = sscanf(zone, "%*s%s", filename);
	chc++;

	//if it is a picture loaded in memory, the allocated space is released
	if ((*st).ok)
		free_all(st);

	FILE *in = fopen(filename, "r");

	if (!in) {
		(*st).ok = 0;

		printf("Failed to load %s\n", filename);
	} else {
		//saving the magic word
		fscanf(in, "%c", &a);
		(*st).mgw[0] = a;

		fscanf(in, "%c", &a);
		(*st).mgw[1] = a;
		(*st).mgw[2] = '\0';

		fscanf(in, "%c", &a);
		skip_comm(in);

		//height and lenght saving
		fscanf(in, "%d%d", &(*st).l, &(*st).h);
		fscanf(in, "%c", &a);
		skip_comm(in);

		//saving the maximum color value
		fscanf(in, "%d", &(*st).col);
		fscanf(in, "%c", &a);
		skip_comm(in);

		//the coordinate area initially represents the whole picture
		(*st).x1 = 0, (*st).x2 = (*st).l, (*st).y1 = 0, (*st).y2 = (*st).h;

		//the picture of the appropriate type is loaded into the memory
		if (strcmp((*st).mgw, "P2") == 0 || strcmp((*st).mgw, "P5") == 0) {
			(*st).gr = alloc_m((*st).h, (*st).l);

			if ((*st).gr) {
				if (strcmp((*st).mgw, "P2") == 0) {
					(*st).ok = 2;

					p2(in, (*st).gr, (*st).h, (*st).l);
				} else {
					(*st).ok = 5;

					p5(in, (*st).gr, (*st).h, (*st).l);
				}

				printf("Loaded %s\n", filename);
			} else {
				(*st).ok = 0;

				printf("Failed to load %s\n", filename);
			}
		} else {
			(*st).r = alloc_m((*st).h, (*st).l);
			(*st).g = alloc_m((*st).h, (*st).l);
			(*st).b = alloc_m((*st).h, (*st).l);

			if ((*st).r && (*st).g && (*st).b) {
				if (strcmp((*st).mgw, "P3") == 0) {
					(*st).ok = 3;

					p3(in, (*st).r, (*st).g, (*st).b, (*st).h, (*st).l);
				} else {
					(*st).ok = 6;

					p6(in, (*st).r, (*st).g, (*st).b, (*st).h, (*st).l);
				}

				printf("Loaded %s\n", filename);
			} else {
				(*st).ok = 0;
				free_all(st);
				printf("Failed to load %s\n", filename);
			}
		}

		fclose(in);
	}
}

void select_z(struct colors *st, char *zone)
{
	int a1, a2, b1, b2, okk = 1;
	int chc = sscanf(zone, "%*s%d%d", &a1, &b1);
	char aa1[256], aa2[256], bb1[256], bb2[256];

	chc = sscanf(zone, "%*s%*s%*s%d%d", &a2, &b2);
	chc = sscanf(zone, "%*s%s%s", aa1, bb1);
	chc = sscanf(zone, "%*s%*s%*s%s%s", aa2, bb2);
	chc++;

	//it is checked if the entered coordinates are of type int
	//otherwise the command is invalid
	if (aa1[0] == '-' && (aa1[1] >= '0' && aa1[1] <= '9')) {
		for (size_t i = 2; i < strlen(aa1); i++)
			if (aa1[i] < '0' || aa1[i] > '9')
				okk = 0;
	} else if (aa1[0] != '-') {
		for (size_t i = 0; i < strlen(aa1); i++)
			if (aa1[i] < '0' || aa1[i] > '9')
				okk = 0;
	}
	
	if (aa2[0] == '-' && (aa2[1] >= '0' && aa2[1] <= '9')) {
		for (size_t i = 2; i < strlen(aa2); i++)
			if (aa2[i] < '0' || aa2[i] > '9')
				okk = 0;
	} else if (aa2[0] != '-') {
		for (size_t i = 0; i < strlen(aa2); i++)
			if (aa2[i] < '0' || aa2[i] > '9')
				okk = 0;
	}

	if (bb1[0] == '-' && (bb1[1] >= '0' && bb1[1] <= '9')) {
		for (size_t i = 2; i < strlen(bb1); i++)
			if (bb1[i] < '0' || bb1[i] > '9')
				okk = 0;
	} else if (bb1[0] != '-') {
		for (size_t i = 0; i < strlen(bb1); i++)
			if (bb1[i] < '0' || bb1[i] > '9')
				okk = 0;
	}

	if (bb2[0] == '-' && (bb2[1] >= '0' && bb2[1] <= '9')) {
		for (size_t i = 2; i < strlen(bb2); i++)
			if (bb2[i] < '0' || bb2[i] > '9')
				okk = 0;
	} else if (bb2[0] != '-') {
		for (size_t i = 0; i < strlen(bb2); i++)
			if (bb2[i] < '0' || bb2[i] > '9')
				okk = 0;
	}

	if (okk == 0) {
		printf("Invalid command\n");
		//it is checked if the entered coordinates belong to the image size range
	} else if (a1 < 0 || a1 > (*st).l || a2 < 0 || a2 > (*st).l) {
		printf("Invalid set of coordinates\n");
	} else if (b1 < 0 || b1 > (*st).h || b2 < 0 || b2 > (*st).h) {
		printf("Invalid set of coordinates\n");
	} else if (a1 == a2 || b1 == b2) {
		printf("Invalid set of coordinates\n");
	} else {
		//the correct order of the coordinates is established
		if (a1 > a2) {
			int aux = a1;
			a1 = a2;
			a2 = aux;
		}

		if (b1 > b2) {
			int aux = b1;
			b1 = b2;
			b2 = aux;
		}

		(*st).x1 = a1, (*st).x2 = a2;
		(*st).y1 = b1, (*st).y2 = b2;

		printf("Selected %d %d %d %d\n", a1, b1, a2, b2);
	}
}

void select_all(struct colors *st)
{
	(*st).x1 = 0, (*st).x2 = (*st).l;
	(*st).y1 = 0, (*st).y2 = (*st).h;
	printf("Selected ALL\n");
}

int histogram(struct colors *st, char *zone, int nrr)
{
	if ((*st).ok == 0) {
		printf("No image loaded\n");
	} else if (nrr != 3) {
		printf("Invalid command\n");
	} else if ((*st).ok == 3 || (*st).ok == 6) {
		printf("Black and white image needed\n");
	} else {
		int x, y;
		int chc = sscanf(zone, "%*s%d%d", &x, &y);
		chc++;

		//the number of values that will be saved in a bin is calculated
		int interv = ((*st).col + 1) / y;
		int *frecv = calloc(((*st).col + 1), sizeof(int));

		if (!frecv)
			return 0;

		//the frequency vector for the pixel values is saved
		for (int i = 0; i < (*st).h; i++)
			for (int j = 0; j < (*st).l; j++)
				frecv[(*st).gr[i][j]]++;

		int *frv = calloc(y, sizeof(int));

		if (!frv) {
			free(frecv);
			return 0;
		}

		//the frequency matrix corresponding to the number of bins is saved
		int cont = 0, nr = 0, maxf = 0;

		for (int i = 0; i <= (*st).col; i++) {
			frv[nr] += frecv[i];

			if (frv[nr] > maxf)
				maxf = frv[nr];

			cont++;

			if (cont == interv) {
				nr++;
				cont = 0;
			}
		}

		free(frecv);

		//the required histogram is displayed
		for (int i = 0; i < y; i++) {
			double dou = frv[i] * x;
			dou /= maxf;
			int nr = dou;

			printf("%d\t|\t", nr);
			for (int j = 0; j < nr; j++)
				printf("*");
			printf("\n");
		}

		free(frv);
	}

	return 0;
}

int equalize(struct colors *st, int nrr)
{
	if ((*st).ok == 0) {
		printf("No image loaded\n");
	} else if (nrr > 1) {
		printf("Invalid command\n");
	} else if ((*st).ok == 3 || (*st).ok == 6) {
		printf("Black and white image needed\n");
	} else {
		double area = (*st).h * (*st).l;
		int *frecv = calloc((*st).col + 1, sizeof(int));

		if (!frecv)
			return 0;

		for (int i = 0; i < (*st).h; i++)
			for (int j = 0; j < (*st).l; j++)
				frecv[(*st).gr[i][j]]++;

		for (int i = 0; i < (*st).h; i++)
			for (int j = 0; j < (*st).l; j++) {
				double val = 255 / area;
				double sum = 0;

				for (int w = 0; w <= (*st).gr[i][j]; w++)
					sum += frecv[w];

				val *= sum;
				int new_val;

				if (val > 255) {
					new_val = 255;
				} else if (val < 0) {
					new_val = 0;
				} else {
					int aux = val;
					float auxx = aux + 0.5;

					if (val < auxx)
						new_val = val;
					else
						new_val = val + 1;
				}

				(*st).gr[i][j] = new_val;
			}

		free(frecv);
		printf("Equalize done\n");
	}

	return 0;
}

int check_selection(struct colors *st)
{
	int a1 = (*st).x1, a2 = (*st).x2;
	int b1 = (*st).y1, b2 = (*st).y2;

	if (a1 == 0 && b1 == 0)
		if (a2 == (*st).l && b2 == (*st).h)
			return 1;
		else
			return 2;
	else
		return 2;
}

int r_180(struct colors *st)
{
	int cz, **grr = NULL, **rr = NULL, **gg = NULL, **bb = NULL;

	if ((*st).ok == 2 || (*st).ok == 5)
		cz = 1;
	else
		cz = 2;

	if (cz == 1) {
		grr = alloc_m((*st).h, (*st).l);
		if (!grr)
			return 0;
	} else {
		rr = alloc_m((*st).h, (*st).l);
		gg = alloc_m((*st).h, (*st).l);
		bb = alloc_m((*st).h, (*st).l);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).h);
			free_m(gg, (*st).h);
			free_m(bb, (*st).h);

			return 0;
		}
	}

	int a = (*st).x1, b = (*st).x2;
	int c = (*st).y1, d = (*st).y2;

	for (int i = c; i < d; i++)
		for (int j = a; j < b; j++) {
			if (cz == 1) {
				grr[i][j] = (*st).gr[c + d - 1 - i][a + b - 1 - j];
			} else {
				rr[i][j] = (*st).r[c + d - 1 - i][a + b - 1 - j];
				gg[i][j] = (*st).g[c + d - 1 - i][a + b - 1 - j];
				bb[i][j] = (*st).b[c + d - 1 - i][a + b - 1 - j];
			}
		}

	for (int i = c; i < d; i++)
		for (int j = a; j < b; j++) {
			if (cz == 1) {
				(*st).gr[i][j] = grr[i][j];
			} else {
				(*st).r[i][j] = rr[i][j];
				(*st).g[i][j] = gg[i][j];
				(*st).b[i][j] = bb[i][j];
			}
		}

	if (cz == 1) {
		free_m(grr, (*st).h);
	} else {
		free_m(rr, (*st).h);
		free_m(gg, (*st).h);
		free_m(bb, (*st).h);
	}

	return 1;
}

int r_90_all(struct colors *st)
{
	int cz, **grr = NULL, **rr = NULL, **gg = NULL, **bb = NULL;

	if ((*st).ok == 2 || (*st).ok == 5)
		cz = 1;
	else
		cz = 2;

	if (cz == 1) {
		grr = alloc_m((*st).l, (*st).h);

		if (!grr)
			return 0;
	} else {
		rr = alloc_m((*st).l, (*st).h);
		gg = alloc_m((*st).l, (*st).h);
		bb = alloc_m((*st).l, (*st).h);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).l);
			free_m(gg, (*st).l);
			free_m(bb, (*st).l);

			return 0;
		}
	}

	int aux = (*st).h;
	(*st).h = (*st).l;
	(*st).l = aux;
	int d = (*st).l, b = (*st).h;

	for (int i = 0; i < b; i++)
		for (int j = 0; j < d; j++) {
			if (cz == 1) {
				grr[i][j] = (*st).gr[d - 1 - j][i];
			} else {
				rr[i][j] = (*st).r[d - 1 - j][i];
				gg[i][j] = (*st).g[d - 1 - j][i];
				bb[i][j] = (*st).b[d - 1 - j][i];
			}
		}

	if (cz == 1) {
		free_m((*st).gr, (*st).l);

		(*st).gr = grr;
	} else {
		free_m((*st).r, (*st).l);
		free_m((*st).g, (*st).l);
		free_m((*st).b, (*st).l);

		(*st).r = rr, (*st).g = gg, (*st).b = bb;
	}

	(*st).x1 = 0, (*st).y1 = 0;
	(*st).x2 = (*st).l, (*st).y2 = (*st).h;

	return 1;
}

int r_90_sel(struct colors *st)
{
	int cz, **grr = NULL, **rr = NULL, **gg = NULL, **bb = NULL;

	if ((*st).ok == 2 || (*st).ok == 5)
		cz = 1;
	else
		cz = 2;

	if (cz == 1) {
		grr = alloc_m((*st).h, (*st).l);
		if (!grr)
			return 0;
	} else {
		rr = alloc_m((*st).h, (*st).l);
		gg = alloc_m((*st).h, (*st).l);
		bb = alloc_m((*st).h, (*st).l);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).h);
			free_m(gg, (*st).h);
			free_m(bb, (*st).h);

			return 0;
		}
	}

	int c = (*st).x1, d = (*st).x2;
	int a = (*st).y1, b = (*st).y2;

	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++) {
			if (cz == 1) {
				grr[i][j] = (*st).gr[a + d - 1 - j][c + i - a];
			} else {
				rr[i][j] = (*st).r[a + d - 1 - j][c + i - a];
				gg[i][j] = (*st).g[a + d - 1 - j][c + i - a];
				bb[i][j] = (*st).b[a + d - 1 - j][c + i - a];
			}
		}

	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++) {
			if (cz == 1) {
				(*st).gr[i][j] = grr[i][j];
			} else {
				(*st).r[i][j] = rr[i][j];
				(*st).g[i][j] = gg[i][j];
				(*st).b[i][j] = bb[i][j];
			}
		}

	if (cz == 1) {
		free_m(grr, (*st).h);
	} else {
		free_m(rr, (*st).h);
		free_m(gg, (*st).h);
		free_m(bb, (*st).h);
	}

	return 1;
}

int r_270_all(struct colors *st)
{
	int cz, **grr = NULL, **rr = NULL, **gg = NULL, **bb = NULL;

	if ((*st).ok == 2 || (*st).ok == 5)
		cz = 1;
	else
		cz = 2;

	if (cz == 1) {
		grr = alloc_m((*st).l, (*st).h);
		if (!grr)
			return 0;
	} else {
		rr = alloc_m((*st).l, (*st).h);
		gg = alloc_m((*st).l, (*st).h);
		bb = alloc_m((*st).l, (*st).h);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).l);
			free_m(gg, (*st).l);
			free_m(bb, (*st).l);

			return 0;
		}
	}

	int aux = (*st).h;
	(*st).h = (*st).l;
	(*st).l = aux;
	int d = (*st).l, b = (*st).h;

	for (int i = 0; i < b; i++)
		for (int j = 0; j < d; j++) {
			if (cz == 1) {
				grr[i][j] = (*st).gr[j][b - 1 - i];
			} else {
				rr[i][j] = (*st).r[j][b - 1 - i];
				gg[i][j] = (*st).g[j][b - 1 - i];
				bb[i][j] = (*st).b[j][b - 1 - i];
			}
		}

	if (cz == 1) {
		free_m((*st).gr, (*st).l);

		(*st).gr = grr;
	} else {
		free_m((*st).r, (*st).l);
		free_m((*st).g, (*st).l);
		free_m((*st).b, (*st).l);

		(*st).r = rr, (*st).g = gg, (*st).b = bb;
	}

	(*st).x1 = 0, (*st).y1 = 0;
	(*st).x2 = (*st).l, (*st).y2 = (*st).h;

	return 1;
}

int r_270_sel(struct colors *st)
{
	int cz, **grr = NULL, **rr = NULL, **gg = NULL, **bb = NULL;

	if ((*st).ok == 2 || (*st).ok == 5)
		cz = 1;
	else
		cz = 2;

	if (cz == 1) {
		grr = alloc_m((*st).h, (*st).l);
		if (!grr)
			return 0;
	} else {
		rr = alloc_m((*st).h, (*st).l);
		gg = alloc_m((*st).h, (*st).l);
		bb = alloc_m((*st).h, (*st).l);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).h);
			free_m(gg, (*st).h);
			free_m(bb, (*st).h);

			return 0;
		}
	}

	int c = (*st).x1, d = (*st).x2;
	int a = (*st).y1, b = (*st).y2;

	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++) {
			if (cz == 1) {
				grr[i][j] = (*st).gr[a + j - c][b - 1 - i + c];
			} else {
				rr[i][j] = (*st).r[a + j - c][b - 1 - i + c];
				gg[i][j] = (*st).g[a + j - c][b - 1 - i + c];
				bb[i][j] = (*st).b[a + j - c][b - 1 - i + c];
			}
		}

	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++) {
			if (cz == 1) {
				(*st).gr[i][j] = grr[i][j];
			} else {
				(*st).r[i][j] = rr[i][j];
				(*st).g[i][j] = gg[i][j];
				(*st).b[i][j] = bb[i][j];
			}
		}

	if (cz == 1) {
		free_m(grr, (*st).h);
	} else {
		free_m(rr, (*st).h);
		free_m(gg, (*st).h);
		free_m(bb, (*st).h);
	}

	return 1;
}

int rotate(struct colors *st, char *zone, int nrr)
{
	int angle, m = 0;
	int chc = sscanf(zone, "%*s%d", &angle);
	chc++;

	if ((*st).ok == 0) {
		printf("No image loaded\n");

		return 0;
	}

	if (nrr != 2) {
		printf("Invalid command\n");

		return 0;
	}

	int caz = check_selection(st);
	if (caz == 2 && (((*st).x2 - (*st).x1) != ((*st).y2 - (*st).y1))) {
		printf("The selection must be square\n");
		return 0;
	}

	if (angle == 0 || angle == 360 || angle == (-360)) {
		printf("Rotated %d\n", angle);
	} else if (angle == 180 || angle == (-180)) {
		m = r_180(st);

		if (m)
			printf("Rotated %d\n", angle);
	} else if (angle == 90 || angle == (-270)) {
		if (caz == 1)
			m = r_90_all(st);
		else
			m = r_90_sel(st);

		if (m)
			printf("Rotated %d\n", angle);
	} else if (angle == 270 || angle == (-90)) {
		if (caz == 1)
			m = r_270_all(st);
		else
			m = r_270_sel(st);

		if (m)
			printf("Rotated %d\n", angle);
	} else {
		printf("Unsupported rotation angle\n");
	}

	return 0;
}

int crop(struct colors *st, int nrr)
{
	if ((*st).ok == 0) {
		printf("No image loaded\n");
	} else if (nrr > 1) {
		printf("Invalid command\n");
	} else {
		int new_h = (*st).y2 - (*st).y1;
		int new_l = (*st).x2 - (*st).x1;

		if ((*st).ok == 2 || (*st).ok == 5) {
			int **new_m = alloc_m(new_h, new_l);

			if (!new_m)
				return 0;

			for (int i = 0; i < new_h; i++)
				for (int j = 0; j < new_l; j++)
					new_m[i][j] = (*st).gr[i + (*st).y1][j + (*st).x1];

			free_all(st);

			(*st).gr = new_m;
			(*st).h = new_h, (*st).l = new_l;
			(*st).y1 = 0, (*st).y2 = new_h;
			(*st).x1 = 0, (*st).x2 = new_l;

			printf("Image cropped\n");
		} else {
			int **rr = alloc_m(new_h, new_l);
			int **gg = alloc_m(new_h, new_l);
			int **bb = alloc_m(new_h, new_l);

			if (rr && gg && bb) {
				for (int i = 0; i < new_h; i++)
					for (int j = 0; j < new_l; j++) {
						rr[i][j] = (*st).r[i + (*st).y1][j + (*st).x1];
						gg[i][j] = (*st).g[i + (*st).y1][j + (*st).x1];
						bb[i][j] = (*st).b[i + (*st).y1][j + (*st).x1];
					}

				free_all(st);

				(*st).r = rr;
				(*st).g = gg;
				(*st).b = bb;
				(*st).h = new_h, (*st).l = new_l;
				(*st).y1 = 0, (*st).y2 = new_h;
				(*st).x1 = 0, (*st).x2 = new_l;

				printf("Image cropped\n");
			} else {
				free_m(rr, new_h);
				free_m(gg, new_h);
				free_m(bb, new_h);

				return 0;
			}
		}
	}

	return 0;
}

int a_red(struct colors *st, int i, int j, int p)
{
	double val = 0;

	if (p == 1) {
		val = (*st).r[i][j] * 8 - (*st).r[i - 1][j] - (*st).r[i + 1][j];
		val += (-1) * ((*st).r[i][j - 1] + (*st).r[i][j + 1]);
		val += (-1) * ((*st).r[i - 1][j - 1] + (*st).r[i + 1][j + 1]);
		val += (-1) * ((*st).r[i - 1][j + 1] + (*st).r[i + 1][j - 1]);
	} else if (p == 2) {
		val = (*st).r[i][j] * 5 - (*st).r[i - 1][j] - (*st).r[i + 1][j];
		val += (-1) * ((*st).r[i][j - 1] + (*st).r[i][j + 1]);
	} else if (p == 3) {
		val = (*st).r[i][j] + (*st).r[i - 1][j] + (*st).r[i + 1][j];
		val += (*st).r[i][j - 1] + (*st).r[i][j + 1];
		val += (*st).r[i - 1][j - 1] + (*st).r[i + 1][j + 1];
		val += (*st).r[i - 1][j + 1] + (*st).r[i + 1][j - 1];
		val /= 9;
	} else {
		val = (*st).r[i][j] * 4;
		val += 2 * ((*st).r[i - 1][j] + (*st).r[i + 1][j]);
		val += 2 * ((*st).r[i][j - 1] + (*st).r[i][j + 1]);
		val += (*st).r[i - 1][j - 1] + (*st).r[i + 1][j + 1];
		val += (*st).r[i - 1][j + 1] + (*st).r[i + 1][j - 1];
		val /= 16;
	}

	int new_val;

	if (val > 255) {
		new_val = 255;
	} else if (val < 0) {
		new_val = 0;
	} else {
		int aux = val;
		float auxx = aux + 0.5;

		if (val < auxx)
			new_val = val;
		else
			new_val = val + 1;
	}

	return new_val;
}

int a_green(struct colors *st, int i, int j, int p)
{
	double val = 0;
	
	if (p == 1) {
		val = (*st).g[i][j] * 8 - (*st).g[i - 1][j] - (*st).g[i + 1][j];
		val += (-1) * ((*st).g[i][j - 1] + (*st).g[i][j + 1]);
		val += (-1) * ((*st).g[i - 1][j - 1] + (*st).g[i + 1][j + 1]);
		val += (-1) * ((*st).g[i - 1][j + 1] + (*st).g[i + 1][j - 1]);
	} else if (p == 2) {
		val = (*st).g[i][j] * 5 - (*st).g[i - 1][j] - (*st).g[i + 1][j];
		val += (-1) * ((*st).g[i][j - 1] + (*st).g[i][j + 1]);
	} else if (p == 3) {
		val = (*st).g[i][j] + (*st).g[i - 1][j] + (*st).g[i + 1][j];
		val += (*st).g[i][j - 1] + (*st).g[i][j + 1];
		val += (*st).g[i - 1][j - 1] + (*st).g[i + 1][j + 1];
		val += (*st).g[i - 1][j + 1] + (*st).g[i + 1][j - 1];
		val /= 9;
	} else {
		val = (*st).g[i][j] * 4;
		val += 2 * ((*st).g[i - 1][j] + (*st).g[i + 1][j]);
		val += 2 * ((*st).g[i][j - 1] + (*st).g[i][j + 1]);
		val += (*st).g[i - 1][j - 1] + (*st).g[i + 1][j + 1];
		val += (*st).g[i - 1][j + 1] + (*st).g[i + 1][j - 1];
		val /= 16;
	}

	int new_val;

	if (val > 255) {
		new_val = 255;
	} else if (val < 0) {
		new_val = 0;
	} else {
		int aux = val;
		float auxx = aux + 0.5;

		if (val < auxx)
			new_val = val;
		else
			new_val = val + 1;
	}

	return new_val;
}

int a_blue(struct colors *st, int i, int j, int p)
{
	double val = 0;

	if (p == 1) {
		val = (*st).b[i][j] * 8 - (*st).b[i - 1][j] - (*st).b[i + 1][j];
		val += (-1) * ((*st).b[i][j - 1] + (*st).b[i][j + 1]);
		val += (-1) * ((*st).b[i - 1][j - 1] + (*st).b[i + 1][j + 1]);
		val += (-1) * ((*st).b[i - 1][j + 1] + (*st).b[i + 1][j - 1]);
	} else if (p == 2) {
		val = (*st).b[i][j] * 5 - (*st).b[i - 1][j] - (*st).b[i + 1][j];
		val += (-1) * ((*st).b[i][j - 1] + (*st).b[i][j + 1]);
	} else if (p == 3) {
		val = (*st).b[i][j] + (*st).b[i - 1][j] + (*st).b[i + 1][j];
		val += (*st).b[i][j - 1] + (*st).b[i][j + 1];
		val += (*st).b[i - 1][j - 1] + (*st).b[i + 1][j + 1];
		val += (*st).b[i - 1][j + 1] + (*st).b[i + 1][j - 1];
		val /= 9;
	} else {
		val = (*st).b[i][j] * 4;
		val += 2 * ((*st).b[i - 1][j] + (*st).b[i + 1][j]);
		val += 2 * ((*st).b[i][j - 1] + (*st).b[i][j + 1]);
		val += (*st).b[i - 1][j - 1] + (*st).b[i + 1][j + 1];
		val += (*st).b[i - 1][j + 1] + (*st).b[i + 1][j - 1];
		val /= 16;
	}

	int new_val;

	if (val > 255) {
		new_val = 255;
	} else if (val < 0) {
		new_val = 0;
	} else {
		int aux = val;
		float auxx = aux + 0.5;

		if (val < auxx)
			new_val = val;
		else
			new_val = val + 1;
	}

	return new_val;
}

void help_apply(int *a1, int *a2, int *b1, int *b2, struct colors *st)
{
	if ((*st).x1 == 0)
		*a1 = 1;
	else
		*a1 = (*st).x1;

	if ((*st).y1 == 0)
		*b1 = 1;
	else
		*b1 = (*st).y1;

	if ((*st).x2 == (*st).l)
		*a2 = (*st).l - 1;
	else
		*a2 = (*st).x2;

	if ((*st).y2 == (*st).h)
		*b2 = (*st).h - 1;
	else
		*b2 = (*st).y2;
}

int apply(struct colors *st, char *zone, int nrr)
{
	if ((*st).ok == 0) {
		printf("No image loaded\n");
	} else if (nrr != 2) {
		printf("Invalid command\n");
	} else if ((*st).ok == 2 || (*st).ok == 5) {
		printf("Easy, Charlie Chaplin\n");
	} else {
		char param[256];
		int a1 = 0, a2 = 0, b1 = 0, b2 = 0;
		help_apply(&a1, &a2, &b1, &b2, st);

		int **rr = alloc_m((*st).h, (*st).l);
		int **gg = alloc_m((*st).h, (*st).l);
		int **bb = alloc_m((*st).h, (*st).l);

		if (!(rr && gg && bb)) {
			free_m(rr, (*st).h), free_m(gg, (*st).h), free_m(bb, (*st).h);

			return 0;
		}

		sscanf(zone, "%*s%s", param);

		if (strcmp(param, "EDGE") == 0) {
			for (int i = b1; i < b2; i++)
				for (int j = a1; j < a2; j++) {
					rr[i][j] = a_red(st, i, j, 1);
					gg[i][j] = a_green(st, i, j, 1);
					bb[i][j] = a_blue(st, i, j, 1);
				}

			printf("APPLY %s done\n", param);
		} else if (strcmp(param, "SHARPEN") == 0) {
			for (int i = b1; i < b2; i++)
				for (int j = a1; j < a2; j++) {
					rr[i][j] = a_red(st, i, j, 2);
					gg[i][j] = a_green(st, i, j, 2);
					bb[i][j] = a_blue(st, i, j, 2);
				}

			printf("APPLY %s done\n", param);
		} else if (strcmp(param, "BLUR") == 0) {
			for (int i = b1; i < b2; i++)
				for (int j = a1; j < a2; j++) {
					rr[i][j] = a_red(st, i, j, 3);
					gg[i][j] = a_green(st, i, j, 3);
					bb[i][j] = a_blue(st, i, j, 3);
				}

			printf("APPLY %s done\n", param);
		} else if (strcmp(param, "GAUSSIAN_BLUR") == 0) {
			for (int i = b1; i < b2; i++)
				for (int j = a1; j < a2; j++) {
					rr[i][j] = a_red(st, i, j, 4);
					gg[i][j] = a_green(st, i, j, 4);
					bb[i][j] = a_blue(st, i, j, 4);
				}

			printf("APPLY %s done\n", param);
		} else {
			printf("APPLY parameter invalid\n");

			return 0;
		}

		for (int i = b1; i < b2; i++)
			for (int j = a1; j < a2; j++) {
				(*st).r[i][j] = rr[i][j];
				(*st).g[i][j] = gg[i][j];
				(*st).b[i][j] = bb[i][j];
			}

		free_m(rr, (*st).h), free_m(gg, (*st).h), free_m(bb, (*st).h);
	}

	return 0;
}

int save(struct colors *st, char *zone, int nrr)
{
	if ((*st).ok == 0) {
		printf("No image loaded\n");
	} else if (nrr != 2 && nrr != 3) {
		printf("Invalid command\n");
	} else {
		char filename[256], op[256];
		int chc = sscanf(zone, "%*s%s", filename);
		chc++;

		FILE *out = fopen(filename, "w");
		if (!out)
			return 0;

		chc = sscanf(zone, "%*s%*s%s", op);

		//it is checked whether the picture will be saved ascii or binary
		if (strlen(filename) + 6 != strlen(zone)) {
			if ((*st).ok == 2 || (*st).ok == 5) {
				fprintf(out, "%s\n", "P2");
				fprintf(out, "%d %d\n%d\n", (*st).l, (*st).h, (*st).col);
				
				for (int i = 0; i < (*st).h; i++) {
					for (int j = 0; j < (*st).l; j++)
						fprintf(out, "%d ", (*st).gr[i][j]);
					fprintf(out, "\n");
				}
			} else {
				fprintf(out, "%s\n", "P3");
				fprintf(out, "%d %d\n%d\n", (*st).l, (*st).h, (*st).col);

				for (int i = 0; i < (*st).h; i++) {
					for (int j = 0; j < (*st).l; j++) {
						fprintf(out, "%d ", (*st).r[i][j]);
						fprintf(out, "%d ", (*st).g[i][j]);
						fprintf(out, "%d ", (*st).b[i][j]);
					}

					fprintf(out, "\n");
				}
			}
		} else {
			if ((*st).ok == 2 || (*st).ok == 5) {
				fprintf(out, "%s\n", "P5");
				fprintf(out, "%d %d\n%d\n", (*st).l, (*st).h, (*st).col);

				for (int i = 0; i < (*st).h; i++) {
					for (int j = 0; j < (*st).l; j++)
						fwrite(&(*st).gr[i][j], sizeof(char), 1, out);
				}
			} else {
				fprintf(out, "%s\n", "P6");
				fprintf(out, "%d %d\n%d\n", (*st).l, (*st).h, (*st).col);

				for (int i = 0; i < (*st).h; i++) {
					for (int j = 0; j < (*st).l; j++) {
						fwrite(&(*st).r[i][j], sizeof(char), 1, out);
						fwrite(&(*st).g[i][j], sizeof(char), 1, out);
						fwrite(&(*st).b[i][j], sizeof(char), 1, out);
					}
				}
			}
		}

		printf("Saved %s\n", filename);
		fclose(out);
	}

	return 0;
}
