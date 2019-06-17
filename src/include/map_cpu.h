//Analog cpu structure
typedef struct Core {
	int core_id;
	int* processor;
}Core;

typedef struct Socket {
	int socket_id;
	Core* core;
}Socket;

typedef struct CPU_info {
	Socket* socket;
	int  socket_num, core_num, proc_num, ht_num;
}CPU_info;
//mapping table data structure is used to store the correspondence between threads and core.	authorï¼hh


typedef struct index {
	int proc_no;
	int thread_no;
}Index;


//read cpu_info
CPU_info* get_cpu_info()
{
	FILE *fp;
	fp = fopen("../out.txt", "r");
	int socket_num, core_num, proc_num, ht_num;
	//Initialize the relevant data structure
	fscanf(fp, "%d%d%d%d", &socket_num, &core_num, &proc_num, &ht_num);
	CPU_info* cpu_info = (CPU_info*)malloc(sizeof(CPU_info));
	cpu_info->socket_num = socket_num;
	cpu_info->core_num = core_num;
	cpu_info->proc_num = proc_num;
	cpu_info->ht_num = ht_num;
	cpu_info->socket = (Socket*)malloc(socket_num*sizeof(Socket));
	//i,j,k are a loop control variables.
	int i, j, k;
	for (i = 0; i < socket_num; i++) {
		cpu_info->socket[i].core = (Core*)malloc(core_num*sizeof(Core));
		for (j = 0; j < core_num; j++) {
			cpu_info->socket[i].core[j].processor = (int*)malloc(ht_num*sizeof(int));
		}
	}
	for (i = 0; i < socket_num; i++) {
		fscanf(fp, "%d", &cpu_info->socket[i].socket_id);
		for (j = 0; j < core_num; j++) {
			fscanf(fp, "%d", &cpu_info->socket[i].core[j].core_id);
			for ( k = 0; k < ht_num; k++)
			{
				fscanf(fp, "%d", &cpu_info->socket[i].core[j].processor[k]);
			}
		}
	}
	return cpu_info;
}
//read config file
void read_config(Index* index) {
	int n, i;
	FILE *fp;
	fp = fopen("benchmark.conf", "r");
	if (fp == NULL) {
		printf("file open error\n");
	}

	fscanf(fp, "%d", &n);
	if (n <= 0)
	{
		fclose(fp);
		printf("number error\n");
	}
	index = (Index*)malloc(sizeof(Index) * n);
	for (i = 0; i < n; i++)
		if (fscanf(fp, "%d%d", &index[i].thread_no, &index[i].proc_no) != 2) break;

	fclose(fp);
}

//mapping function: 
Index* mapping(int thread_num, int type) {
	Index* index;
	if (type == 8) {//User-defined mode
		read_config(index);
	}
	else
	{
		index= (Index*)malloc(sizeof(Index) * thread_num);
		CPU_info* c = get_cpu_info();
		int i, s_c,c_c,p_c;//Set the corresponding loop variable;
		switch (type)
		{
		case 0://This mode is used to bind all threads to one hyperthread of a single core of a single socket;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[0].core[0].processor[0];
				index[i].thread_no = i;
			}
			break;
		case 1://This mode is used to bind all threads to all hyperthreads of a single core of a single socket;
			p_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[0].core[0].processor[p_c];
				index[i].thread_no = i;
				p_c++;
				if (p_c >= c->ht_num) {
					p_c = 0;
				}
			}
			break;
		case 2://This mode is used to bind all threads to a hyperthread on a different core of a single socket;
			c_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[0].core[c_c].processor[0];
				index[i].thread_no = i;
				c_c++;
				if (c_c >= c->core_num) {
					c_c = 0;
				}
			}
			break;
		case 3://This mode is used to bind all threads to all hyperthreads on different cores of a single socket;
			c_c = 0;
			p_c= 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[0].core[c_c].processor[p_c];
				index[i].thread_no = i;
				p_c++;
				if (p_c >= c->ht_num) {
					c_c++;
					p_c = 0;
				}
				if (c_c >= c->core_num)
					c_c = 0;
			}
			break;
		case 4://This mode is used to bind all threads to one hyperthread of a single core of a different socket;
			s_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[s_c].core[0].processor[0];
				index[i].thread_no = i;
				s_c++;
				if (s_c >= c->socket_num) {
					s_c = 0;
				}
			}
			break;
		case 5://This mode is used to bind all threads to all hyperthreads of a single core of different sockets;
			s_c = 0;
			p_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[s_c].core[0].processor[p_c];
				index[i].thread_no = i;
				p_c++;
				if (p_c >= c->ht_num) {
					s_c++;
					p_c = 0;
				}
				if (s_c >= c->socket_num)
					s_c = 0;
			}
			break;
		case 6://This mode is used to bind all threads to one hyperthread of a different core of a different socket;
			s_c = 0;
			c_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[s_c].core[c_c].processor[0];
				index[i].thread_no = i;
				s_c++;
				if (s_c >= c->socket_num) {
					c_c++;
					s_c = 0;
				}
				if (c_c >= c->core_num)
					c_c = 0;
			}
			break;
		case 7://This mode is used to bind all threads to all hyperthreads on different cores of different sockets;
			s_c = 0,c_c=0;
			p_c = 0;
			for (i = 0; i < thread_num; i++) {
				index[i].proc_no = c->socket[s_c].core[c_c].processor[p_c];
				index[i].thread_no = i;
				s_c++;
                if (s_c >= c->socket_num) {
					c_c++;
                    s_c = 0;
                }
				if (c_c >= c->core_num) {
					p_c++;                                         
					c_c = 0;                                 
				}
				if (p_c >= c->ht_num)
					p_c = 0;
			}
			break;
		default:
			printf("type is invalid!");
			break;
		}
	}
	return index;
}
//Get the number corresponding to the thread
int get_proc_no(Index* index, int thread_no) {
	return index[thread_no].proc_no;
}
