

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<string.h>
#include <stdint.h>
#include <TlHelp32.h> // для process_check()
// traverse - обход по всем элементам
int main();
int Check_CRCs();
char password[100];


// получает указатель на jmp на функцию в таблице, возвращает указатель на функцию.
void* get_func_addr(void* address) {
	uint8_t* ptr = (uint8_t*)address;
	if (ptr[0] == 0xE9) {
		int32_t offset = *(int32_t*)(ptr + 1);
		return ptr + 5 + offset;
	}
	return address;
}

void generate_code(char* code) {
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		code[i] = '0' + rand() % 10;
	}
	code[10] = '\0';
}

// 
// mode: 1 - зашифровать, 0 - расшифровать. знаки ( и ) не использовать!
// csr == caesar decrypt
unsigned char* csr(unsigned const char str[512]) {
	volatile int shift = 11; // на самом деле 13. меняем в self_modifying
	int mode = 0;
	if (!mode) shift = -shift;
	static unsigned char output[512];
	int length = strlen((char*)str);
	char* symbols = "(7410852963:._$%+ )";

	for (int i = 0; i < length + 1; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			// Обработка заглавных букв
			output[i] = 'A' + (str[i] - 'A' + shift) % 26;
			if (output[i] < 'A') output[i] += 26; // Коррекция для отрицательного сдвига
		}
		else if (str[i] >= 'a' && str[i] <= 'z') {
			// Обработка строчных букв
			output[i] = 'a' + (str[i] - 'a' + shift) % 26;
			if (output[i] < 'a') output[i] += 26; // Коррекция для отрицательного сдвига
		}
		else if (0) {
			// цифры и спец символы
			int j = 0;
			int len = strlen(symbols);
			while (j < len) {
				if (str[i] == symbols[j]) break;
				j++;
			}

			if (j < len) {
				if (shift > 0) output[i] = symbols[j + 1];
				else output[i] = symbols[j - 1];
			}
		}
		// Все остальные символы остаются без изменений
		else output[i] = str[i];
		
	}
	return output;
}


#pragma auto_inline(off)

// 1 - если пароль в файле и переданный пароль совпадают, 0 - нет
// вставлена фейковая проверка. на самом деле функция отработала верно если вернула значение в файле 
long long check_password(char* password) {
	int a;
	FILE* file1;
	file1 = fopen(csr("cnffjbeq.gkg"), "r");
	char file_pswd[100];
	fgets(file_pswd, 100, file1);

	char serial[11];
	FILE* file2;
	file2 = fopen(csr("frevny.gkg"), "w");
	generate_code(serial);

	// ложная проверка
	if (!strcmp(file_pswd, csr("snxr_cnffjbeq"))) {
		// неправда, это нифига не успех
		puts(csr("Fhppfrf. Pbqr unf orra trarengrq"));
		fprintf(file2, csr("XRL$s^1XWzc%$"));
		return 1;
	}

	
	
	if (!strcmp(file_pswd, password)) {
		fprintf(file2, csr("XRL$%f$"), serial);
		// return atoi(serial);
	}
	else {
		puts(csr("Cnffjbeq vf vapbeerpg"));
		return 0;
	}
	fclose(file2);
	// тот самый нужный вывод
	return atoi(serial);
}

// считает atoi от серийника в serial.txt. Если они совпадают, то ложная проверка в check_password не прошла (её не взломали)
int real_check(long long check_result) {
	FILE* f2 = fopen(csr("frevny.gkg"), "r");
	char* serial_str = malloc(20);
	fgets(serial_str, 20, f2);
	serial_str = serial_str + 4;
	serial_str[10] = 0;

	if (atoi(serial_str) != check_result) {
		puts(csr("Cnffjbeq unpxrq"));
		return 0;
	}
	fclose(f2);
	return 1;
}

typedef struct sosiska {
	int year;
	int mon;
	int day;
	int hour;
	int min;

	char day_of_week[100];
	char description[2000];
	char place[1000];
	int importance;

	struct Node* pLeft;
	struct Node* pRight;
} Node;

Node* Date_Root = NULL;  // корень дерева сортировки по дате
Node* Imp_Root = NULL;  // корень дерева сортировки по важности
Node* Place_Root = NULL;  // корень дерева сортировки по месту

Node* createNewNode(Node item) {
	Node* p = (Node*)malloc(sizeof(Node));
	p->year = item.year;
	p->mon = item.mon;
	p->day = item.day;
	p->hour = item.hour;
	p->min = item.min;
	strcpy(p->day_of_week, item.day_of_week);
	strcpy(p->description, item.description);
	strcpy(p->place, item.place);
	p->importance = item.importance;

	p->pLeft = NULL;
	p->pRight = NULL;
	return p;
}

void add_to_Imp_tree(Node item) {
	if (Imp_Root == NULL) {
		Imp_Root = createNewNode(item);
	}
	else {
		Node* p = Imp_Root;
		while (1) {
			if (p->importance >= item.importance) {
				if (p->pLeft == NULL) {
					p->pLeft = createNewNode(item);
					break;
				}
				p = p->pLeft;
			}
			else {
				if (p->pRight == NULL) {
					p->pRight = createNewNode(item);
					break;
				}
				p = p->pRight;
			}
		}
	}
}

void add_to_Date_tree(Node item) {
	if (Date_Root == NULL) {
		Date_Root = createNewNode(item);
	}
	else {
		Node* p = Date_Root;
		long long event_time_item = ((item.year - 1970) * 525600) + item.mon * 43800 + item.day * 1400 + item.hour * 60 + item.min - 27180; // в конце поправочка на високосный год и всяеую фигню
		while (1) {
			long long event_time_p = ((p->year - 1970) * 525600) + p->mon * 43800 + p->day * 1400 + p->hour * 60 + p->min - 27180; // в конце поправочка для корректности
			if (event_time_p >= event_time_item) {
				if (p->pLeft == NULL) {
					p->pLeft = createNewNode(item);
					break;
				}
				p = p->pLeft;
			}
			else {
				if (p->pRight == NULL) {
					p->pRight = createNewNode(item);
					break;
				}
				p = p->pRight;
			}
		}
	}
}

void add_to_Place_tree(Node item) {
	if (Place_Root == NULL) {
		Place_Root = createNewNode(item);
	}
	else {
		Node* p = Place_Root;
		while (1) {
			if (strcmp(p->place, item.place) >= 0) {  // значение первой строки больше чем значение второй
				if (p->pLeft == NULL) {
					p->pLeft = createNewNode(item);
					break;
				}
				p = p->pLeft;
			}
			else {
				if (p->pRight == NULL) {
					p->pRight = createNewNode(item);
					break;
				}
				p = p->pRight;
			}
		}
	}
}

void Traverse(Node * p, char* str1)
{
	
	// дополнительная проверка 9-d
	FILE* f1 = fopen(csr("cnffjbeq.gkg"), "r");
	char str2[100];
	fgets(str2, 100, f1);
	if (strcmp(str1, str2) != 0) {
		while (1) printf(csr("Cnffjbeq unpxrq."));
	}

	if (p == NULL) return;
	Traverse(p->pLeft, str1);

	printf("%d.%d.%d %s", p->day, p->mon, p->year, p->day_of_week); // в day_of_week включен символ \n
	printf("%d:", p->hour);
	if (p->min < 10) printf("0"); // чтобы 13:01 не выглядело как 13:1
	printf("%d\n", p->min);

	printf(csr("Qrfpevcgvba: "));
	fputs(p->description, stdout);
	printf(csr("Cynpr: "));
	fputs(p->place, stdout);

	printf("Importance: %d\n", p->importance);
	printf("\n");

	Traverse(p->pRight, str1);

}

int if_data_correct(Node item) {
	int date_correct = 0;

	time_t current_time = time(NULL) / 60;

	long long event_time;
	//считаем введенное время в тиках
	event_time = ((item.year - 1970) * 525600 + item.mon * 43800 + item.day * 1400 + item.hour * 60 + item.min) - 27180; // в конце поправочка
	if (event_time > current_time) {
		if ((item.year < 3000)) {
			if ((item.mon <= 12)) {
				if ((item.hour <= 24)) { // проверка на корректность отдельно снизу - с учетом месяца
					if ((item.mon == 2) && (item.day <= 29)) {
						if (item.year % 4 == 0) {
							if (item.year % 100 == 0) {
								if (item.year % 400 == 0) {
									if (item.day <= 29) date_correct = 1;  // год делится на 400
								}
							}
							else {
								if (item.day <= 29) date_correct = 1; // год делится на 4 и не делится на 100
							}
						}
						else {
							if (item.day <= 28) date_correct = 1;  // невисокосный год
						}
					}
					else if (item.mon % 2 == 1) {
						if (item.day <= 31) date_correct = 1; // нечетный месяц
					}
					else {
						if (item.day <= 30) date_correct = 1; // четный месяц
					}
				}
			}
		}
	}
	if (date_correct == 0) {
		printf(csr("Qngr vapbeerpg, gel ntnva. (Lrne zhfg or yrff guna 3000)\n"));
		return 0;
	}
	if ((item.importance > 10) || (item.importance < 0)) {
		//printf("%d", item.importance);
		printf(csr("Vapbeerpg vzcbegnapr. Vachg inyhr sebz 0 gb 10. Gel ntnva.\n"));
		return 0;
	}
	return 1;
}

int isLeapYear(int year) {

	if (year % 400 == 0) {
		return 1;
	}
	if (year % 100 == 0) {
		return 0;
	}
	if (year % 4 == 0) {
		return 1;
	}
	return 0;
}

void getDayOfWeek(int year, int month, int day, char* arr) {
	int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (isLeapYear(year)) {
		monthDays[1] = 29;
	}

	// Calculate the total number of days from the base date (e.g., 01/01/0000)
	long long totalDays = day;
	for (int i = 0; i < month - 1; i++) {
		totalDays += monthDays[i];
	}
	for (int i = 0; i < year; i++) {
		totalDays += (isLeapYear(i) ? 366 : 365);
	}

	// Day of the week calculation
	int dayOfWeek = totalDays % 7;
	char dayNames[10][50] = { "Friday\n", "Saturday\n", "Sunday\n", "Monday\n", "Tuesday\n", "Wednesday\n", "Thursday\n" };

	if (dayOfWeek == 0) strcpy(arr, dayNames[0]);
	else if (dayOfWeek == 1) strcpy(arr, dayNames[1]);
	else if (dayOfWeek == 2) strcpy(arr, dayNames[2]);
	else if (dayOfWeek == 3) strcpy(arr, dayNames[3]);
	else if (dayOfWeek == 4) strcpy(arr, dayNames[4]);
	else if (dayOfWeek == 5) strcpy(arr, dayNames[5]);
	else if (dayOfWeek == 6) strcpy(arr, dayNames[6]);
}

Node ask_for_new_object() {
	Node curr_item;

	do {
		printf(csr("vachg lrne bs rirag: "));
		scanf("%d", &(curr_item.year));
		printf(csr("vachg zbhagu bs rirag:"));
		scanf("%d", &(curr_item.mon));
		printf(csr("vachg qnl bs rirag:"));
		scanf("%d", &(curr_item.day));
		printf(csr("vachg ubhe bs rirag: "));
		scanf("%d", &(curr_item.hour));
		printf(csr("vachg zvahgr bs rirag:"));
		scanf("%d", &(curr_item.min));

		char aaaa[213];  // хз почему но не хочет работать gets с первого включения
		fgets(aaaa, sizeof(aaaa), stdin); // понял)) - он забирает какой-то символ \n
		getDayOfWeek(curr_item.year, curr_item.mon, curr_item.day, curr_item.day_of_week);

		printf(csr("vachg qrfpevcgvba bs rirag:"));
		fgets(curr_item.description, sizeof(curr_item.description), stdin);
		//gets(curr_item.description);
		printf(csr("vachg cynpr bs rirag:"));
		fgets(curr_item.place, sizeof(curr_item.place), stdin);
		//gets(curr_item.place);
		printf(csr("vachg vzcbegnapr bs rirag:"));
		scanf("%d", &(curr_item.importance));

		if (if_data_correct(curr_item) == 1) break;
	} while (1);

	return curr_item;
}

Node take_item_from_file(FILE * file) {
	Node item;
	fscanf(file, "%d\n", &(item.year));
	fscanf(file, "%d\n", &(item.mon));
	fscanf(file, "%d\n", &(item.day));
	fscanf(file, "%d\n", &(item.hour));
	fscanf(file, "%d\n", &(item.min));
	fgets(item.day_of_week, 100, file);
	fgets(item.description, 2000, file);
	fgets(item.place, 1000, file);
	fscanf(file, "%d\n", &(item.importance));

	return item;
}

void save_tree(FILE * file, Node * p) {
	if (p == NULL) return;
	save_tree(file, p->pLeft);
	fprintf(file, "%d\n", p->year);
	fprintf(file, "%d\n", p->mon);
	fprintf(file, "%d\n", p->day);
	fprintf(file, "%d\n", p->hour);
	fprintf(file, "%d\n", p->min);

	fputs(p->day_of_week, file);
	fputs(p->description, file);
	fputs(p->place, file);
	fprintf(file, "%d\n", p->importance);
	save_tree(file, p->pRight);

}

Node* search_by_time(Node item, Node * p, int* left_or_right) {
	Node* parent = NULL;
	long long event_time_item = 60 * ((item.year - 1970) * 525600 + item.mon * 43800 + item.day * 1400 + item.hour * 60 + item.min) - 1630800; // в конце поправочка
	while (1) {
		long long event_time_p = 60 * ((p->year - 1970) * 525600 + p->mon * 43800 + p->day * 1400 + p->hour * 60 + p->min) - 1630800; // в конце поправочка для корректности
		if (item.importance <= p->importance) {
			if (event_time_item == event_time_p) {
				return parent;
				break;
			}
			*left_or_right = 0;
			parent = p;
			p = p->pLeft;
		}
		else {
			if (event_time_item == event_time_p) {
				return parent;
				break;
			}
			*left_or_right = 1;
			parent = p;
			p = p->pRight;
		}
	}
	printf(csr("Qngr vapbeerpg\n"));
	return NULL;
}

void copy_item(Node * destination, Node * sourse) { // копирует p2 в p1
	(destination)->year = (sourse)->year;
	(destination)->mon = (sourse)->mon;
	(destination)->day = (sourse)->day;
	(destination)->hour = (sourse)->hour;
	(destination)->min = (sourse)->min;
	strcpy((destination)->day_of_week, (sourse)->day_of_week);
	strcpy((destination)->description, (sourse)->description);
	strcpy((destination)->place, (sourse)->place);
	(destination)->importance = (sourse)->importance;
}

void delete_event(Node itemToDelete) {
	// сначала удалим из дерева Imp_Root
	int left_or_right = 0; // 0 - если удаляем левого ребенка, 1 - если правого
	Node* parent = search_by_time(itemToDelete, Imp_Root, &left_or_right);

	Node* p;
	// если не потомок (удаляем вершину)
	if (parent == NULL) {
		parent = Imp_Root;
		if (parent->pLeft == NULL && parent->pRight == NULL) {
			Imp_Root = NULL;
			return;
		}
		else if (parent->pLeft != NULL) {
			p = parent->pLeft;
			if (p->pLeft == NULL && p->pRight == NULL) {
				copy_item(parent, p);
				parent->pLeft = NULL;
				return;
			}
		}
		else {
			Imp_Root = Imp_Root->pRight;
			return;
		}
	}

	// если потомок слева
	else if (left_or_right == 0) {
		p = parent->pLeft;
		if ((p->pLeft == NULL) && (p->pRight == NULL)) {
			parent->pLeft = NULL;
			return;
		}
	}

	//если потомок справа
	else {
		p = parent->pRight;
		if ((p->pLeft == NULL) && (p->pRight == NULL)) {
			parent->pRight = NULL;
			return;
		}
	}

	// элемент, который поместим на место удаляемого
	Node* replaced_item = p;
	Node* parent_of_rp = parent;
	if (replaced_item->pLeft == NULL) {
		parent_of_rp = replaced_item;
		replaced_item = p->pRight;

		//нашли максимальный элемент, теперь копируем его значения в текущий элемент, а сам элемент потом удаляем
		copy_item(p, replaced_item);

		parent_of_rp->pRight = NULL;
	}

	// если есть два потомка - ищем максимальный элемент в левом поддереве (и потом ставим его на место удаляемого элемента
	else {
		parent_of_rp = replaced_item;
		replaced_item = replaced_item->pLeft;

		// если потомков нет, то элемент слева от p - максимальный в левом поддереве (единственный)
		if ((replaced_item->pLeft == NULL) && (replaced_item->pRight == NULL)) {
			//нашли максимальный элемент, теперь копируем его значения в текущий элемент, а сам элемент потом удаляем
			copy_item(p, replaced_item);

			parent_of_rp->pLeft = NULL;
		}
		else {
			// двигаемся вправо до упора по левому поддереву (и берем с конца максимальный элемент)
			while ((replaced_item)->pRight != NULL) {
				parent_of_rp = replaced_item;
				replaced_item = ((replaced_item)->pRight);
			}
			//нашли максимальный элемент, теперь копируем его значения в текущий элемент, а сам элемент потом удаляем
			copy_item(p, replaced_item);

			parent_of_rp->pRight = NULL;
		}
	}
}

int isFileEmpty(FILE * file) {
	// сохраняем текущую позицию
	int currentPosition = ftell(file);
	// находим конец файла
	fseek(file, 0, SEEK_END);
	// берем позицию конца файла
	int endPosition = ftell(file);
	// восстанавливаем изначальную позицию
	fseek(file, currentPosition, SEEK_SET);
	// проверяем файл на пустоту
	return (endPosition == 0);
}



void patch_byte(void* address, uint8_t value) {
	DWORD oldProtect;

	// Разрешаем запись
	if (VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		*(uint8_t*)address = value; // запись байта
		// Восстанавливаем старые права
		VirtualProtect(address, 1, oldProtect, &oldProtect);
	}
	else {
		printf("VirtualProtect failed\n");
	}
}

void download_file(FILE * file) {
	if (isFileEmpty(file) == 1) return; // проверка файла на пустоту 
	
	// загрузка информации из файла
	// 11-d (по хорошему дожно быть feof(file))
	while (feof(file)) {
		Node curr_item = take_item_from_file(file);
		add_to_Imp_tree(curr_item);
		add_to_Date_tree(curr_item);
		add_to_Place_tree(curr_item);
	}
}

void reload_tree() {

	// очищаем содержимое файла
	FILE* file1 = fopen("knizhka.txt", "w");
	save_tree(file1, Imp_Root);
	fclose(file1);
	// очищаем деревья
	Date_Root = NULL;  // корень дерева сортировки по дате
	Imp_Root = NULL;  // корень дерева сортировки по важности
	Place_Root = NULL;  // корень дерева сортировки по месту
	// загружаем обновленный файл
	FILE* file2 = fopen("knizhka.txt", "r");
	download_file(file2);
	fclose(file2);
}

// поиск точки останова в памяти (int 3 - байт 0хСС). 11-a 1/4
int CheckForCCBreackpoint(void* pMemory, size_t SizeToCheck) {
	unsigned char* pTmp = (unsigned char*)pMemory;
	for (size_t i = 0; i < SizeToCheck; i++) {
		if (pTmp[i] == 0xCC) return 1;
	}
	return 0;
}

// проверяет 3 основные функции на наличие точки останова. ПО МЕРЕ ДОБАВЛЕНИЯ КОДА РЕДАКТИРОВАТЬ КОЛ-ВО ПРОВЕРЯЕМЫХ БАЙТ. 11-a 1/4
int check_for_0xCC(int len_check_password, int len_real_check,int len_main) {
	
	void* funcPtr = (void*)check_password;
	funcPtr = get_func_addr(funcPtr);
	if (CheckForCCBreackpoint(funcPtr, len_check_password)) {
		puts(csr("Reebe 11-n-1/4 (1): ghea bss qrohttre"));
		return 1;
	}
	
	funcPtr = (void*)real_check;
	funcPtr = get_func_addr(funcPtr);
	if (CheckForCCBreackpoint(funcPtr, len_real_check)) {
		puts(csr("Reebe 11-n-1/4 (2): ghea bss qrohttre"));
		return 1;
	}
	
	funcPtr = (void*)main;
	funcPtr = get_func_addr(funcPtr);
	if (CheckForCCBreackpoint(funcPtr, len_main)) {
		puts(csr("Reebe 11-n-1/4 (3): ghea bss qrohttre"));
		return 1;
	}
	
	return 0;
}

// 1 если в дебаггере, 0 - нет
int Trap_flag() {
	BOOL isDebugged = TRUE;

	__try {
		__asm {
			pushfd  // put EFLAGS in stack 
			or dword ptr[esp], 0x100  // set TF in EFLAGS
			popfd // put value from head of stack to EFLAGS
			nop                       // after nop, processor will generate single-step exception
		}
	}
	// if there is debugger, it will take this single step
	// if there is no degugger, no one, except program, will get single-step exception, so we get in and isDebugged = FALSE
	__except (EXCEPTION_EXECUTE_HANDLER) {
		isDebugged = FALSE;
	}

	if (isDebugged) {
		puts(csr("Reebe 11-n-2/4: ghea bss qrohttre"));
		while (1);
	}
	else {
		return 0;
	}
}

// проверка флагов процесса. Получаем из TEB (Thread Environment Block) - Process Environment Block (PEB). Читаем поле NtGlobalFlag, если установлен бит 0х70 - мы в дебагере
int NtGlobalFlag() {
	unsigned long NtGlobalFlags = 0;

	__asm {
		mov eax, fs: [30h] ;
		mov eax, [eax + 68h];
		mov NtGlobalFlags, eax;
	}
	if (NtGlobalFlags & 0x70) {
		puts(csr("Reebe 11-n-3/4: ghea bss qrohttre"));
		return 1;
	}
	return 0;
}

// проверяет параметры процесса. 1 - если в отладчике. Подробнее - https://xakep.ru/2018/01/17/antidebug/#toc01.
int Win32_DebuggingApi() {
	BOOL IsDbgPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsDbgPresent);
	if (IsDbgPresent) {
		puts(csr("Reebe 11-n-4/4: ghea bss qrohttre"));
		return 1;
	}
	return 0;
}

//Count CRC. 1000000000000101 = 0x8005 = 32773 - value for CRC16
uint16_t CRC16(uint8_t* ptr, int count)
{
	uint16_t crc;
	uint8_t i;
	crc = 0;

	while (--count >= 0)
	{
		crc = crc ^ ((uint16_t)(*ptr++ << 8));
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
			{
				crc = (crc << 1) ^ 0x8005;
			}
			else
			{
				crc = crc << 1;
			}
		}
	}

	return crc;
}

// 11-с-1/2
// проверяет производителя оборудования. Если мы в ВМ - в реестре запись о ВМ
int producer_check() {
	
	HKEY hkey = NULL;
	LSTATUS ret;
	char value[1024];
	DWORD size = sizeof(value);
	char* way = "SYSTEM\\CurrentControlSet\\Control\\SystemInformation";
	ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, way, 0, KEY_READ, &hkey);
	if (ret == ERROR_SUCCESS) {
		// читаем значение
		RegQueryValueExA(hkey, "SystemProductName", NULL, NULL, (LPBYTE)value, &size);
		// ищем подстроки
		if (strstr(value, "VMware") != 0) {
			printf("Error 11-c-1/2: VMware detected!");
			RegCloseKey(hkey);
			return 1;
		}
		if (strstr(value, "VirtualBox") != 0) {
			printf("Error 11-c-1/2: VirtualBox detected!");
			RegCloseKey(hkey);
			return 1;
		}
	}
	RegCloseKey(hkey);
	return 0;
}

// 11-c-2/2
// Проверяет открытые процессы. Среди них может быть процесс ВМ
int process_check() {
	PROCESSENTRY32 process; // информация о процессе
	process.dwSize = sizeof(PROCESSENTRY32); // необходимо инициировать это поле
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Создаёт «снимок» всех процессов в системе.
	WCHAR* name = L"vmtool.exe"; // Широкая строка (UTF-16). Обычная char не будет работать
	
	if (Process32First(snapshot, &process) == TRUE) { // заполняет process информацией о первом процессе в снимке
		
		// проверяем первый снимок
		if (wcscmp(process.szExeFile, name) == 0) {
			printf("Error 11-c-2/2: VM detected!\n");
			CloseHandle(snapshot);
			return 1;
		}
		// Перебирает последующие записи
		while (Process32Next(snapshot, &process) == TRUE) { 
			if (wcscmp(process.szExeFile, name) == 0) {
				printf("Error 11-c-2/2: VM detected!\n");
				CloseHandle(snapshot);
				return 1;
			}
		}
	}
	CloseHandle(snapshot);
	return 0;
}

// 11-d
int self_modifying() {

	// меняем je на jne в цикле while в dowload_file
	void* funcPtr = (void**)download_file;
	uint8_t* addr = (uint8_t*)get_func_addr(funcPtr);
	addr = (uint8_t*)funcPtr;
	int n = 0;
	while (n < 1)
	{
		if ((*addr) == 0x74) n++;
		addr++;
	}
	addr--;
	patch_byte(addr, 0x75);

	// меняем shift с 11 на 13 в csr
	funcPtr = (void**)csr;
	addr = (uint8_t*)get_func_addr(funcPtr);
	addr = (uint8_t*)funcPtr;
	while ((*addr) != 0x0B) {
		addr++;
	}
	patch_byte(addr, 0x0D);
}


void check_for_hacker() {

	// 11-b 3/3 impossible disassembly
	__asm {
		mov ax, 0x5EB
		xor eax, eax
		__emit 0x74 // jz
		__emit 0xFA // на 6 байт назад

		__emit 0xEB // fake jmp

	}
	
	strcpy(password, csr("1234"));

	int len_check_password = 200;
	int len_real_check = 100;
	int len_main = 100;
	
	// 11-b 3/3 (1) impossible disassembly
	__asm {
		mov ax, 0x5EB
		xor eax, eax
		__emit 0x74 // jz
		__emit 0xFA // на 6 байт назад

		__emit 0xEB // fake jmp

	}

	// 11-e - обфускация потока управления
	__try {
		__asm {
			pushfd  // put EFLAGS in stack 
			or dword ptr[esp], 0x100  // set TF in EFLAGS
			popfd // put value from head of stack to EFLAGS
			nop                       // after nop, processor will generate single-step exception
		}
	}
	// if there is debugger, it will take this single step
	// if there is no degugger, no one, except program, will get single-step exception, so we get in and isDebugged = FALSE
	__except (EXCEPTION_EXECUTE_HANDLER) {
		self_modifying(); // 11-d
	}

	// проверяем целостность функций. 9-c
	if (Check_CRCs()) exit(0);
	
	// проверяем не в отладчике ли мы
	if (check_for_0xCC(len_check_password, len_real_check, len_main)) exit(0); // 11-a 1/4
	if (Trap_flag()) exit(0);	// 11-a 2/4
	if (NtGlobalFlag()) exit(0); // 11-a 3/4
	if (Win32_DebuggingApi()) exit(0); // 11-a 4/4
	
	
	if (producer_check()) exit(0);
	if (process_check()) exit(0);
	
}

int main() {

	// противодействие дизассемблированию. 11-b
	// перепрыгивание опкода инкструкции
	
	// 11-b 1/3 перепрыгивание опкода инструкции. внутри этого блока спрятали вызов функции проверок
	__asm {
		xor eax, eax
		jz label
		__emit 0xE9

		label:
		pop eax
		call check_for_hacker // проверка не пытаются ли нас дебажить / не подменены ли функции
		jmp label2
	}	
	// 11-b 2/3 используем ret для пререхода - прячет label2 и дальнейший код
	__asm {
		call $ + 7
		add[esp], 7
		ret
	}
label2:	

	__asm nop; // после метки нужна иснтрукция

	FILE* file;
	file = fopen(csr("xavmuxn.gkg"), "a+");
	download_file(file);
	fclose(file);

		

	// перед началом работы проверяем пароль 
	long long check_result = check_password(password);
	if (check_result == 0) return 0;

	char input[100];
	// 0 - сохранить и выйти
	// 1 - добавить элемент в дерево
	// 2 - показать список дел, отсортированный по дате
	// 3 - показать список дел, отсортированный по важности
	// 4 - показать список дел, отсортированный по месту 
	// 5 - удалить элемент

	
	// 11-e бинарная обфускация 1/3
	if (file == NULL) {
		puts("BkhGVcjfHVj5utyf5e");
		return 1421;
	}
	
	while (1) {
		
		//printf("choose action:\n0 - save and exit\n1 - add new event\n2 - show list, sorted by date\n3 - show list, sorted by importance\n");
		puts(csr("pubbfr npgvba:"));
		puts(csr("0 - fnir naq rkvg"));
		puts(csr("1 - nqq arj rirag"));
		puts(csr("2 - fubj yvfg, fbegrq ol qngr"));
		puts(csr("3 - fubj yvfg, fbegrq ol vzcbegnapr"));
		puts(csr("4 - fubj yvfg, fbegrq ol cynpr"));
		puts(csr("5 - qryrgr rirag"));
		//printf("4 - show list, sorted by place\n5 - delete event\n");
		scanf("%s", &input);

		
		//11-e бинарная обфускация 2/3. мусорный код
		int mas[15] = {1, 1, 1, 1, 2, 2, 2, 8};
		int x = 2;
		int l = 0, r = 8 - 1;
		while (l < r) {
			if ((r - l) % 2 == 1) {
				if (mas[l] == x) {
					if (0) printf("   ");
					if (0) return 0;
				}
				else l++;
			}
			int mid = (r + l) / 2;
			if (mas[l] == x) {
				if (0) printf("   ");
				if (0) return 0;
			}
			else if (mas[mid] >= x) r = mid;
			else if (mas[mid] <= x) l = mid;
		}
		// конец 11-e 2/3


		// настоящая проверка с подсчетом atoi(serial).  9-e
		if (real_check(check_result) == 0) return 0;
		
		// 11-e бинарная обфускация 3/3
		if ((input[0] == '0' - 1) && input[1] == '\0') {
			printf("LAKJcnjvsjbalfiASSCi13");
			Traverse(Imp_Root, password);
			strcpy(password, "88005553535");
			reload_tree();
		}

		else if (input[0] == '0' && input[1] == '\0') {
			// очищаем содержимое файла
			FILE* file1 = fopen(csr("xavmuxn.gkg"), "w");
			save_tree(file1, Imp_Root);
			fclose(file1);
			exit(0);
		}
		else if (input[0] == '1' && input[1] == '\0') {
			Node curr_item = ask_for_new_object();
			add_to_Imp_tree(curr_item);
			puts(csr("\nVgrz nqqrq gb yvfg\n"));

			reload_tree();
		}
		else if (input[0] == '2' && input[1] == '\0') {
			printf(csr("\nFbegrq ol qngr yvfg bs riragf:\n"));
			Traverse(Date_Root, password);
		}

		else if (input[0] == '3' && input[1] == '\0') {
			printf(csr("\nFbegrq ol vzcbegnapr yvfg bs riragf:\n"));
			Traverse(Imp_Root, password);
		}
		else if (input[0] == '4' && input[1] == '\0') {
			printf(csr("\nFbegrq ol cynpr yvfg bs riragf:\n"));
			Traverse(Place_Root, password);
		}
		else if (input[0] == '5' && input[1] == '\0') {
			Node itemToDelete;
			do {
				printf(csr("\nVachg qngr naq gvzr bs rirag, lbh jnag gb qryrgr (sbezng: ubhe zvahgr qnl zbhagu lrne): "));
				scanf("%d %d %d %d %d", &(itemToDelete.hour), &(itemToDelete.min), &(itemToDelete.day), &(itemToDelete.mon), &(itemToDelete.year));
				printf(csr("Vachg vzcbegnapr bs rirag: "));
				scanf("%d", &(itemToDelete.importance));
			} while (if_data_correct(itemToDelete) == 0);

			delete_event(itemToDelete);

			reload_tree();
		}
		else {
			printf(csr("\nVapbeerpg inyhr. Gel ntnva :)\n"));
		}
	}


	return 0;
}


// сверяет значения CRC с эталонными. 9-c
int Check_CRCs() {

	int check_password_CRC;
	int real_check_CRC;
	int main_CRC;
	FILE* file = fopen(csr("pep.gkg"), "r");

	fscanf(file, "%d", &check_password_CRC);
	fscanf(file, "%d", &real_check_CRC);
	fscanf(file, "%d", &main_CRC);

	void* funcPtr = (void*)check_password;
	funcPtr = get_func_addr(funcPtr);
	if (CRC16(funcPtr, 300) != check_password_CRC) {
		printf("Error 9-c (1): code modified.");
		return 1;
	}

	funcPtr = get_func_addr((void*)real_check);
	if (CRC16(funcPtr, 100) != real_check_CRC) {
		printf("Error 9-c (2): code modified.");
		return 1;
	}

	funcPtr = get_func_addr((void*)main);
	if (CRC16(funcPtr, 500) != main_CRC) {
		printf("Error 9-c (3): code modified.");
		return 1;
	}

	return 0;
}

/*
Данные в файле хранятся в виде (9 строк на одну запись):
2024 - год
12 - месяц
11 - день
1 - час
50 - минута
Saturday - день недели
sosiski - описание
dacha - место
10 - важность
*/








