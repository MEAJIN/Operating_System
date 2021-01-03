#include <stdio.h> // �Լ��� ��ũ�� ����, ����� �Լ� �� ����
#include <stdlib.h> // ���ڿ� ��ȯ, ���� �޸� ����
#include <string.h> // �޸� ����̳� ���ڿ��� �ٷ� �� �ִ� �Լ��� ����
#include <fcntl.h> // ������ ����
#include <sys/mman.h>
#include <unistd.h>

int main() 
{	
	// ���� �޸� ������
	const int SIZE = 4096;

	// ���� �޸�
	const char* name = "OS"; 

	// ���� �޸𸮿� ����
	const char* message0 = "Hello ";
	const char* message1 = "Soonchunhyang ";
	const char* message2 = "Computer Engineering and Science!\n";

	int shm_fd;

	// ���� �޸� ������ > ���� �޸𸮴� �ڷ����� �����Ƿ� void ����
	void *ptr;

	/* ���� �޸� ���� */

	// ����1. ���� �޸� ��ü �̸�
	// ����2. �б�� ���� ���� ����, ��ü�� ���� ���� ������ ���� ���� > ������, ������ ���� ������ �����ϱ� ���� ���� ���� �� �߰� �ʿ�
	// ����3. ��� ���� ���� 0666 > r,w
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 


	/* ���� �޸� ũ�� ���� */
	ftruncate(shm_fd, SIZE);

	/* ���� �޸� ��ü�� �޸𸮿� �����Ͽ� ������ ������ �ּҷ� ��ȯ */

	// ����1. ���� �ּҸ� �����ϴ� ������ > ���������� null�� ����
	// ����2. ����Ǵ� �޸� ũ�� = 4096
	// ����3. �а�,���Ⱑ ������ ������ ǥ��
	// ����4. ���� ���� ǥ��, ���μ����� ���� ������ ǥ��
	// ����5. ������ ��ü�� ���ϱ����
	// ����6. ������ �޸� ������ ���ۿ��������� ������
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (ptr == MAP_FAILED)
	{
		printf("Map failed\n");
		return -1;
	}

	/* ���� ���� �޸� ������ ��� */
	/* �� ���� �Է� ��, ptr�� ���� ���� ���Ѿ� �� */

	sprintf(ptr, "%s", message0);
	ptr += strlen(message0);

	sprintf(ptr, "%s", message1);
	ptr += strlen(message1);

	sprintf(ptr, "%s", message2);
	ptr += strlen(message2);


	return 0;
}
