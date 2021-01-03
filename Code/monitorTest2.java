class monitorTest2  { 
	public static void main(String[] args)
	
	throws InterruptedException {
		BankAccount b = new
		BankAccount();
		Parent p = new Parent(b);
		Child c = new Child(b);
		p.start();
		c.start();
		p.join();
		c.join();
		System.out.println( "\nbalance = " + b.getBalance());
	}
}
	
class BankAccount {
	int balance;
	boolean d_turn = true; // 입금/출금 순서 플래그
	
	synchronized void deposit(int amt) { // 입금 메서드
	int temp = balance + amt;
	
	System.out.print("+");
	balance = temp;
	
	notify();  // 조건대기 출금 스레드 깨움
	d_turn = false;// 출금 차례 지정 
	
	try { // 조건 대기 
		wait();
	} catch (InterruptedException e) {}
}

synchronized void withdraw(int amt) {
	while (d_turn) { // 입금차례이면 조건대기
	
		try { 
			wait();
		} catch(InterruptedException e) {} 
	} 
	
	int temp = balance - amt;
	System.out.print("-");
	balance = temp;
	
	notify();  // 조건대기 입금 스레드 깨움
	d_turn = true; // 입금 차례 지정
	}

	int getBalance() {
		return balance;
	}
}

class BankDeposit extends Thread { // 입금 스레드 클래스
	BankAccount b;
	
	Parent(BankAccount b) {
		this.b = b;
	}
	
	public void run() {
		for (int i=0; i<100; i++)
			b.deposit(1000);
	}
}
	

class BankWithdraw extends Thread {  // 출금 스레드 클래스 
	BankAccount b;
	
	Child(BankAccount b) {
		this.b = b;
	}
	
	public void run() {
		for (int i=0; i<100; i++)
			b.withdraw(1000);
	}
}