#include <stdio.h> // 함수의 메크로 정의, 입출력 함수 등 관리
#include <string.h> // 메모리 블록이나 문자열을 다룰 수 있는 함수를 포함
#include <sys/types.h> // 우분투에서 메모리를 관리
#include <unistd.h> // 다양한 상수와, 자료형, 함수 정의

#define BUFFEF_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void)
{
	char write_msg[BUFFEF_SIZE] = "Greetings";
	char read_msg[BUFFEF_SIZE];
	int fd[2];
	pid_t pid;

	// 파이프 생성
	if (pipe(fd) == -1)
	{
		fprintf(stderr, "Pipe failed");
		return 1;
	}

	// 프로세스 호출
	pid = fork();

	// pid 값이 0보다 작으면 에러 발생 > 1값 반환
	if (pid < 0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	//  pid가 0보다 크면 부모 프로세스
	if (pid > 0) 
	{
		// 사용하지 않는 파이프 종료
		close(fd[READ_END]);

		// 파이프를 씀
		write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);

		// 쓰기가 끝난 파이프 종료
		close(fd[WRITE_END]);
	}

	// 그렇지 않으면 자식 프로세스
	else
	{	
		// 사용하지 않는 파이프 종료
		close(fd[WRITE_END]); 

		// 파이프를 읽음
		read(fd[READ_END], read_msg, BUFFEF_SIZE);
		printf("read %s", read_msg);

		// 읽기가 끝난 파이프 종료
		close(fd[READ_END]);
	}

	return 0;

}



