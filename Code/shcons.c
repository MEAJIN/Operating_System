#include <stdio.h> // 함수의 메크로 정의, 입출력 함수 등 관리
#include <stdlib.h> // 문자열 변환, 동적 메모리 관리
#include <fcntl.h> // 파일을 제어
#include <sys/mman.h> // 우분투에서 메모리를 관리


int main()
{
	// 공유 메모리 사이즈
	const int SIZE = 4096;

	// 공유 메모리
	const char* name = "OS";

	// 프로세스에서 열린 파일의 목록을 관리하는 테이블의 인덱스 > 각 인덱스 값지정 > 파일 지칭 가능
	int shm_fd;

	int i;

	// 공유 메모리 포인터 > 공유 메모리는 자료형이 없으므로 void 선언
	void *ptr;

	/* 공유 메모리 오픈 */

	// 인자1. 공유 메모리 객체 이름
	// 인자2. 읽기 전용으로 오픈
	// 인자3. 사용 권한 모드는 0666 > r,w
	shm_fd = shm_open(name, O_RDONLY, 0666);


	// -1과 같으면 오류로 인한 종류로 구분
	if (shm_fd == -1)
	{
		printf("shard memory failed\n");
		exit(-1);
	}

	/* 이제 공유 메모리 세그먼트를 프로세스의 주소 공간에 매핑 */

	// mmap()는 성공시, 가져온 영역의 포인터 값을들 반환
	// 인자1. 시작 주소를 제안하는 인자
	// 인자2. 적재되는 메모리 크기 = 4096
	// 인자3. 읽기가 가능한 페이지 표시
	// 인자4. 적재 유형 표시, 프로세스들 간의 공유를 표시
	// 인자5. 적재할 객체의 파일기술자
	// 인자6. 적재할 메모리 영역의 시작에서부터의 오프셋
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	// -1과 같으면 오류로 인한 종류로 구분
	if (shm_fd == -1)
	{
		printf("shard memory failed\n");
		exit(-1);
	}

	/* 이제 공유 메모리 영역에 기록 */

	printf("%s", ptr);
	
	/* 공유 메모리 객체 제거 */

	// 공유 메모리를 사용하고 나서는 반드시 unlink를 해야함
	// 왜? 안 해주면 프로세스는 종료되어도 메모리 상의 공유메모리 영역이 제거되지 않을 수 있어서
	// 그렇게 되면 같은 프로세스를 재시작할 때, 정상작동이 되지 않을 수 있음

	// 객체 제거 성공=0, 실패=-1
	if (shm_unlink(name) == -1)
	{
		printf("Error removing %s\n", name);
		exit(-1);
	}

	return 0;
}