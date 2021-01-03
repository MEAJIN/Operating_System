#include <stdlib.h> // 문자열 변환, 동적 메모리 관리
#include <stdio.h> // 함수의 메크로 정의, 입출력 함수 등 관리
#include <time.h> // 시간 관련 함수 라이브러리
#include <pthread.h> // 병렬도 작동하는 S/W를 위해 제공 되는 API => 쓰레드 제어를 위한 함수 제공
#include <semaphore.h> // 세마포어 연산을 사용하는데 필요한 함수
#include <unistd.h> // 다양한 상수와, 자료형, 함수 정의

// 버퍼
// 버퍼는 데이터를 한 곳에서 다른 한 곳으로 전송하는 동안 일시적으로 그 데이터를 보관하는 메모리 영역
typedef int buffer_item;
#define BUFFER_SIZE 7
buffer_item buffer[BUFFER_SIZE];
int insertIndex;
int removeIndex;

// 세마포어 및 뮤텍스
// 세마포어는 공유된 자원의 데이터를 여러 프로세스가 접근 하는 것을 막음
// 뮤텍스는 공유된 자원의 데이터를 여러 쓰레드가 접근 하는 것을 막음
sem_t full;
sem_t empty;
pthread_mutex_t mutex;

// 난수 생성 변수 
unsigned int seed;

// sleep 변수
int SLEEP_TIME;

// 버퍼 출력
void printBuffer() {
    printf("\t\t 버퍼 출력: [");
    int i;
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d ", buffer[i]); 
    }
    printf("]\n");

}

// 버퍼 메소드 => 버퍼 재사용
int insert_item(buffer_item item) {

    if (buffer[insertIndex] != '\0') {
        printf("버퍼에 삽입할 수 없습니다. item이 이미 존재합니다!\n");
        return -1;

    } else {
		// 아이템을 버퍼에 삽입
        buffer[insertIndex] = item;
		
		// insertIndex 위치 조정 => 순환 버퍼 조정 =>  임계구역 접근 방지
        insertIndex++;
        insertIndex = insertIndex % BUFFER_SIZE;

        printf("---> item 생산: %d\n", item);
        printBuffer();
        return 0;

    }

}

int remove_item(buffer_item item) {

    if (buffer[removeIndex] == '\0') {
        printf("버퍼에서 제거할 수 없습니다. item이 존재하지 않습니다!\n");
        return -1;

    } else {
		// 버퍼에서 객체를 제거하여 항목에 넣음
        buffer_item removedItem = buffer[removeIndex];
        buffer[removeIndex] = '\0';

		// removeIndex 위치 조정 => 순환 버퍼 조정 =>  임계구역 접근 방지
        removeIndex++;
        removeIndex = removeIndex % BUFFER_SIZE;

        printf("<--- 소비item : %d\n", removedItem);
        printBuffer();
        return 0;

    }

}

// 여기 부터 버퍼 정지

// 생산자
void *producer(void *param) {
    buffer_item item;
    while (1) {
        printf("여기는 생산자 영역 입니다.\n");
		
		// // 1에서 SLEEP_TIME 사이의 시간 동안 sleep
        sleep(rand_r(&seed) % SLEEP_TIME + 1);
		
		// empty: 버퍼에서 비어있는 공간의 개수(초기값 size)
		// 잠금 수행을 통해 sem에 의해 참조된 세마포어가 현재 잠겨 있지 않다면 잠군다.
		// 해당 값이 0이라면 호출하는 스레드는 세마포어를 잠그거나 시그널에 의해 호출이 중단되기 전 까지는 돌아오지 않음.
        sem_wait(&empty);
		
		// 뮤텍스 잠금 생성 => 호출한 스레드가 뮤텍스 객체를 잠근다
		// 이미 잠겨있다면 호출한 스레드는 뮤텍스 객체가  해지될 떄 까지 대기했다가 뮤텍스 객체를 얻는다. \
		    => 해당 함수가 반환 되었다면 호출한 함수는 뮤텍스 객체를 획득 한 것
		// 성공 = 0, 실패 = error
        pthread_mutex_lock(&mutex);

		// 임계 구역 이라고도 함.
		// 병령 컴퓨팅 에서 둘 이상의 스레드가 동시에 접근해서는 안 되는 공유 자원에 접근 하는 것을 말함.
		// 해당 구역에 들어 가려면 지정된 시간 만큼 대기해야 하는데 이를 sleep로 지정
        printf("생산자 공유 변수 영역 입니다.\n");
		
		// 난수 생성
        item = rand_r(&seed);
		
		// 버퍼에 item을 삽입하고 오류가 있을 경우 아래 문구 출력
        if (insert_item(item) < 0) {
            printf("버퍼에 삽입할 수 없습니다. item이 이미 존재합니다!\n");

        }
		
		// 뮤텍스 잠금 해제
		// 다른 스레드에 의해 잠금을 수행할 수 있음
        pthread_mutex_unlock(&mutex);
		
		// 전체 신호
		// 함수 수행이 성공이면 세마포어의 상태는 잠금상태가 되며, 해당 함수가 실행되어 성공적으로 반환하기 전 까지 잠금 상태 유지
        sem_post(&full);
    }

}

// 소비자
void *consumer(void *param) {
    buffer_item *item = (int *)malloc(sizeof(int));
    while (1) {
        printf("여기는 소비자 영역 입니다.\n");
		
		// 1에서 SLEEP_TIME 사이의 시간 동안 sleep
        sleep(rand_r(&seed) % SLEEP_TIME + 1);
		
		// full : 버퍼에서 차있는 공간의 개수(초기값=0)
		// 잠금 수행을 통해 sem에 의해 참조된 세마포어가 현재 잠겨 있지 않다면 잠군다.
		// 해당 값이 0이라면 호출하는 스레드는 세마포어를 잠그거나 시그널에 의해 호출이 중단되기 전 까지는 돌아오지 않음.
        sem_wait(&full);
		
		// 뮤텍스 잠금 생성 => 호출한 스레드가 뮤텍스 객체를 잠근다
		// 이미 잠겨있다면 호출한 스레드는 뮤텍스 객체가  해지될 떄 까지 대기했다가 뮤텍스 객체를 얻는다. => 해당 함수가 반환 되었다면 호출한 함수는 뮤텍스 객체를 획득 한 것
		// 성공 = 0, 실패 = error
        pthread_mutex_lock(&mutex);
		
		// 임계 구역 이라고도 함.
		// 병령 컴퓨팅 에서 둘 이상의 스레드가 동시에 접근해서는 안 되는 공유 자원에 접근 하는 것을 말함.
		// 해당 구역에 들어 가려면 지정된 시간 만큼 대기해야 하는데 이를 sleep로 지정
        printf("소비자 공유 변수 영역 입니다.\n");
		
		// 난수 생성
        item = rand_r(&seed);
		
		// 버퍼에 item을 삽입하고 오류가 있을 경우 아래 문구 출력
        if (remove_item(*item) < 0) {
            printf("버퍼에서 제거 할 수 없습니다. item이 존재하지 않습니다!\n");
        }
		
		// 여기서부터 공유 변수 영역 종료
		
		// 뮤텍스 잠금 해제
		// 다른 스레드에 의해 잠금을 수행할 수 있음
        pthread_mutex_unlock(&mutex);
		
		// empty: 버퍼에서 비어있는 공간의 개수(초기값 size)
		// 빈 신호
		// 함수 수행이 성공이면 세마포어의 상태는 잠금상태가 되며, 해당 함수가 실행되어 성공적으로 반환하기 전 까지 잠금 상태 유지
        sem_post(&empty);

    }
    free(item);
}


int main(int argc, char*argv[]) {

	// count는 버퍼에 저장된 데이터 개수를 뜻 함
    int producerCount;
    int consumerCount;
	
	// 1. 3가지 인수(x, y, z)를 입력 해준다 > 각 인수는 \n으로 구분 > x : seelp 시간, y : 생상자 수, z : 소비자 수
	// 만약 3가지 인수를 전달 받지 못 했을 경우 아래 문구를 출력 하고 -1 반환
    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("3가지 인수를 입력해 주세요. : sleep 시간, 생산자 수, 소비자 수 (각 인수는 공백으로 구분 짓습니다.)\n");
        return -1;

	// 3가지 인수를 입력 받았을 경우 각 변수에 값을 할당 해줌
    } else {
        SLEEP_TIME = atoi(argv[1]);
        producerCount = atoi(argv[2]);
        consumerCount = atoi(argv[3]);

    }

	// 버퍼, 뮤텍스, 세마포어 및 기타 전역 변수 초기화
    seed = (unsigned int)time(NULL);
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
	
	// 생산자 스레드 생성
    pthread_t producers[producerCount];
    int i;
    for (i = 0; i < producerCount; i++) {
        pthread_attr_t pt_attr;
		
		// 기본 값 할당
        pthread_attr_init(&pt_attr);
	
		// 스레드 생성
        int producerResults = pthread_create(&producers[i], &pt_attr, producer, NULL);
		
		// 생산자 스레드 생성 오류 시 아래 문구 출력
        if (producerResults == -1) {
            printf("생산자 스레드 생성 오류.\n");

        }

    }
	
	// 소비자 스레드 생성
    pthread_t consumers[consumerCount];
    for (i = 0; i < consumerCount; i++) {
        pthread_attr_t ct_attr;
		
		// 기본 값 할당
        pthread_attr_init(&ct_attr);
		
		// 스레드 생성
        int consumerResults = pthread_create(&consumers[i], &ct_attr, consumer, NULL);
		
		// 소비자 스레드 생성 오류 시 아래 문구 출력
        if (consumerResults == -1) {
            printf("소비자 스레드 생성 오류.\n");

        }

    }

	// sleep
    printf("Sleeping...\n");
    sleep(SLEEP_TIME);
	
	// 뮤텍스 및 세마포어 제거
    for(i = 0; i < producerCount; i++)
        pthread_cancel(producers[i]);
    for(i = 0; i < consumerCount; i++)
        pthread_cancel(consumers[i]);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
	
	// 종료
    return 0;

}

