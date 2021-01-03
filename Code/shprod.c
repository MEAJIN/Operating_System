#include <stdio.h> // 함수의 메크로 정의, 입출력 함수 등 관리
#include <stdlib.h> // 문자열 변환, 동적 메모리 관리
#include <string.h> // 메모리 블록이나 문자열을 다룰 수 있는 함수를 포함
#include <fcntl.h> // 파일을 제어
#include <sys/mman.h>
#include <unistd.h>

int main() 
{	
	// 공유 메모리 사이즈
	const int SIZE = 4096;

	// 공유 메모리
	const char* name = "OS"; 

	// 공유 메모리에 적재
	const char* message0 = "Hello ";
	const char* message1 = "Soonchunhyang ";
	const char* message2 = "Computer Engineering and Science!\n";

	int shm_fd;

	// 공유 메모리 포인터 > 공유 메모리는 자료형이 없으므로 void 선언
	void *ptr;

	/* 공유 메모리 생성 */

	// 인자1. 공유 메모리 객체 이름
	// 인자2. 읽기와 쓰기 상태 오픈, 객체가 존재 하지 않으면 파일 생성 > 생성시, 파일의 접근 권한을 지정하기 위해 접근 권한 값 추가 필요
	// 인자3. 사용 권한 모드는 0666 > r,w
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 


	/* 공유 메모리 크기 구성 */
	ftruncate(shm_fd, SIZE);

	/* 공유 메모리 객체를 메모리에 적재하여 접근할 포인터 주소로 반환 */

	// 인자1. 시작 주소를 제안하는 인자임 > 보편적으로 null값 지정
	// 인자2. 적재되는 메모리 크기 = 4096
	// 인자3. 읽고,쓰기가 가능한 페이지 표시
	// 인자4. 적재 유형 표시, 프로세스들 간의 공유를 표시
	// 인자5. 적재할 객체의 파일기술자
	// 인자6. 적재할 메모리 영역의 시작에서부터의 오프셋
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (ptr == MAP_FAILED)
	{
		printf("Map failed\n");
		return -1;
	}

	/* 이제 공유 메모리 영역에 기록 */
	/* 각 문자 입력 후, ptr의 값을 증가 시켜야 함 */

	sprintf(ptr, "%s", message0);
	ptr += strlen(message0);

	sprintf(ptr, "%s", message1);
	ptr += strlen(message1);

	sprintf(ptr, "%s", message2);
	ptr += strlen(message2);


	return 0;
}
