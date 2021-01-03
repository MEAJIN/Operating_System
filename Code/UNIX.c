#include <stdio.h> // �Լ��� ��ũ�� ����, ����� �Լ� �� ����
#include <string.h> // �޸� ����̳� ���ڿ��� �ٷ� �� �ִ� �Լ��� ����
#include <sys/types.h> // ��������� �޸𸮸� ����
#include <unistd.h> // �پ��� �����, �ڷ���, �Լ� ����

#define BUFFEF_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void)
{
	char write_msg[BUFFEF_SIZE] = "Greetings";
	char read_msg[BUFFEF_SIZE];
	int fd[2];
	pid_t pid;

	// ������ ����
	if (pipe(fd) == -1)
	{
		fprintf(stderr, "Pipe failed");
		return 1;
	}

	// ���μ��� ȣ��
	pid = fork();

	// pid ���� 0���� ������ ���� �߻� > 1�� ��ȯ
	if (pid < 0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	//  pid�� 0���� ũ�� �θ� ���μ���
	if (pid > 0) 
	{
		// ������� �ʴ� ������ ����
		close(fd[READ_END]);

		// �������� ��
		write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);

		// ���Ⱑ ���� ������ ����
		close(fd[WRITE_END]);
	}

	// �׷��� ������ �ڽ� ���μ���
	else
	{	
		// ������� �ʴ� ������ ����
		close(fd[WRITE_END]); 

		// �������� ����
		read(fd[READ_END], read_msg, BUFFEF_SIZE);
		printf("read %s", read_msg);

		// �бⰡ ���� ������ ����
		close(fd[READ_END]);
	}

	return 0;

}



