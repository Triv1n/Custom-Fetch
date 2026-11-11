#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRINT_FORMAT "\033[1m\033[36m%-10s\033[0m : %s\n"

const char *penguin[] = {
	"	o_0	",
	"      (oo)	",
	"    //---\\	",
	"   /[:||:]\\	",
	"     I  I	",
};


struct PCInfo {
	char cpu_model_name[100];
	char distro_name[100];
	char uptime[256];
	char memInfo[256];
};

int readCPUInfo(struct PCInfo *cpuinfo) {
	FILE *cpuInfoFile = fopen("/proc/cpuinfo", "r");
	
	char buffer[256];
	if(cpuInfoFile == NULL) {
		printf("Error! cpuinfo can't be readed!");
		return 1;
	}

	while(fgets(buffer, sizeof(buffer), cpuInfoFile)) {
		if(strncmp(buffer, "model name", 10) == 0) {
			sscanf(buffer, "model name : %[^\n]", cpuinfo->cpu_model_name);
			break;
		}
	}

	fclose(cpuInfoFile);
	return 0;
}

int readDistro(struct PCInfo *distroInfo) {
	FILE *distroNameInfo = fopen("/etc/os-release", "r");
	
	char buffer[256];
	
	if(distroNameInfo == NULL) {
		printf("Error! os-release can't be readed!");
		return 1;		
	}

	while(fgets(buffer, sizeof(buffer), distroNameInfo)) {
		if(strncmp(buffer, "NAME", 4) == 0) {
			sscanf(buffer, "NAME=\"%[^\"]\"", distroInfo->distro_name);
			break;
		}
	}

	fclose(distroNameInfo);

	return 0;
}

int readMem(struct PCInfo *memInfo) {
	FILE *memInfoFile = fopen("/proc/meminfo", "r");
	
	char buffer[256];
	long memTotal = 0;
	long memAvailable = 0;

	if(memInfoFile == NULL) {
		printf("Error! Memory information can't be readed!");
		return 1;
	}

	while(fgets(buffer, sizeof(buffer), memInfoFile)) {
		if(strncmp(buffer, "MemTotal", 8) == 0) {
			sscanf(buffer, "MemTotal: %ld", &memTotal);
		}
		else if(strncmp(buffer, "MemAvailable", 12) == 0) {
			sscanf(buffer, "MemAvailable: %ld", &memAvailable);
		}
	}

	memTotal /= 1024;
	memAvailable /= 1024;
	
	snprintf(memInfo->memInfo, sizeof(memInfo->memInfo), "Total: %ld MB, Available: %ld MB", memTotal, memAvailable);

	fclose(memInfoFile);
	return 0;
}

int readUptime(struct PCInfo *uptimeInfo) {
	FILE *uptime = fopen("/proc/uptime", "r");
	
	double uptime_seconds = 0;

	if(uptime == NULL) {
		printf("Error! uptime can't be readed!");
		return 1;		
	}
	
	fscanf(uptime, "%lf", &uptime_seconds);
	fclose(uptime);

	int total_minutes = (int)(uptime_seconds / 60);
	int hours = total_minutes / 60;
	int minutes = total_minutes % 60;
	
	snprintf(uptimeInfo->uptime, sizeof(uptimeInfo->uptime), "%d hours, %d minutes", hours, minutes);
	
	return 0;

}

void printFetch(struct PCInfo *PCInfo) {
	printf("%s	", penguin[0]); printf(PRINT_FORMAT, "Distro", PCInfo->distro_name);
	printf("%s	", penguin[1]);	printf(PRINT_FORMAT, "Processor", PCInfo->cpu_model_name);
	printf("%s	", penguin[2]); printf(PRINT_FORMAT, "Uptime", PCInfo->uptime);
	printf("%s	", penguin[3]); printf(PRINT_FORMAT, "Memory", PCInfo->memInfo);
	printf("%s	\n", penguin[4]);
}

int main() {
	struct PCInfo myPCInfo;
	
	readDistro(&myPCInfo);
	readCPUInfo(&myPCInfo);
	readMem(&myPCInfo);
	readUptime(&myPCInfo);

	printFetch(&myPCInfo);

	return 0;
}
