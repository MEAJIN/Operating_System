class monitorTest1 {
	public static void main(String[] args)
	throws InterruptedException {
		BankAccount b = new BankAccount();
		BankDeposit d = new BankDeposit(b);  // 입금 스레드
		BankWithdraw w = new BankWithdraw(b); // 출금 스레드
		
		d.start();  w.start();
		d.join();  w.join();
		
		System.out.println( "\nbalance = " + b.getBalance());
	}
}

class BankAccount {
	int balance;
	
	synchronized void deposit(int amt) { // 입금 메서드
	int temp = balance + amt;
	
	System.out.print("+"); balance = temp;
}

// 출금 메서드 
	synchronized void withdraw(int amt) {
	int temp = balance - amt;
	
	System.out.print("-"); balance = temp;
}

	int getBalance() {
		return balance; 
	}
}

// 입금 스레드 클래스
class BankDeposit extends Thread {
	BankAccount b;

	BankDeposit(BankAccount b) {
	this.b = b; }

	public void run() {
		for (int i=0; i<100; i++) b.deposit(1000);
	}
}

// 출금 스레드 클래스
class BankWithdraw extends Thread {
	BankAccount b;

	BankWithdraw(BankAccount b) {
		this.b = b; }

	public void run() {
		for (int i=0; i<100; i++)
			b.withdraw(1000);
	}
}