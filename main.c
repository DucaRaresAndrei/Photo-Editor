//Duca Andrei-Rares
//311CA
#include <functions.h>

int main(void)
{
	char zone[256], comm[256], aux[256], *p;
	struct colors st;

	//no picture has been uploaded yet
	st.ok = 0;

	//the program stops when it no longer receives commands
	while (fgets(zone, sizeof(zone), stdin)) {
		strcpy(aux, zone);
		aux[strlen(aux) - 1] = '\0';

		int nr = 0;
		p = strtok(aux, " ");

		while (p) {
			nr++;
			p = strtok(NULL, " ");
		}

		sscanf(zone, "%s", comm);

		if (strcmp(comm, "LOAD") == 0) {
			if (nr == 1 || nr > 2)
				printf("Invalid command\n");
			else
				load(&st, zone);
		} else if (strcmp(comm, "SELECT") == 0) {
			if (st.ok == 0) {
				printf("No image loaded\n");
				continue;
			}

			char comm2[30];
			int chc = sscanf(zone, "%*s%s", comm2);
			chc++;

			if (strcmp(comm2, "ALL") == 0) {
				if (nr == 2)
					select_all(&st);
				else
					printf("Invalid command\n");
			} else {
				if (nr == 5)
					select_z(&st, zone);
				else
					printf("Invalid command\n");
			}
		} else if (strcmp(comm, "HISTOGRAM") == 0) {
			histogram(&st, zone, nr);
		} else if (strcmp(comm, "EQUALIZE") == 0) {
			equalize(&st, nr);
		} else if (strcmp(comm, "ROTATE") == 0) {
			rotate(&st, zone, nr);
		} else if (strcmp(comm, "CROP") == 0) {
			crop(&st, nr);
		} else if (strcmp(comm, "APPLY") == 0) {
			apply(&st, zone, nr);
		} else if (strcmp(comm, "SAVE") == 0) {
			save(&st, zone, nr);
		} else if (strcmp(comm, "EXIT") == 0) {
			if (st.ok == 0) {
				printf("No image loaded\n");
			} else {
				free_all(&st);
				st.ok = 0;
				break;
			}
		} else {
			printf("Invalid command\n");
		}
	}

	//if the program no longer receives commands,
	//and a picture is still loaded in memory, the memory is freed
	if (st.ok != 0)
		free_all(&st);
		
	return 0;
}
