
module atm (input clk,
            input rst,
            input BTN3, BTN2, BTN1, Language,
            input [3:0] value,
            input [3:0] pin , 
            input [3:0] pin1 ,
            input [3:0] pin2 ,
            input [3:0] cardnumber);
    
    //WIRES USED

    reg [3:0] card [14:0]; 
    reg [3:0] password[14:0];  
    reg [3:0] balance [14:0]; 
    reg passTrue;
    reg cardTrue;
    reg[3:0] balancedisplay;
	reg [3:0] current_state;
	reg [3:0] next_state;
    reg start_timer;
    wire time_up;
    reg start_timer1;
    wire time_up1;
    integer i;
    reg [3:0] foundloc;
    reg DepositDone, WithDone, BalDone;


    // PARAMETERS
	parameter [3:0] IDLE = 4'b0000; // doing nothing
    parameter [3:0] CardCheck = 4'b0001;
    parameter [3:0] PASS_ENT_3 = 4'b0010;
	parameter [3:0] PASS_ENT_2 = 4'b0011;
	parameter [3:0] PASS_ENT_1 = 4'b0100;
	parameter [3:0] LOCK = 4'b0101; // aquire lock if pass is incorrect 3 times
	parameter [3:0] ATM_MENU = 4'b0110; // state where the user can choose his operation : 1. Withdraw 2. Deposit 3. Balance
	parameter [3:0] Deposit = 4'b0111; 
	parameter [3:0] Withdraw = 4'b1000; 
	parameter [3:0] Balance = 4'b1001; 



initial
begin
readall;
end

task readall;
begin
$readmemb("G:/Semester 5/Electronic Design Automation/Project1/cardVerilog.txt", card);
$readmemb("G:/Semester 5/Electronic Design Automation/Project1/balanceVerilog.txt", balance);
$readmemb("G:/Semester 5/Electronic Design Automation/Project1/passVerilog.txt", password);
end
endtask

task writeBalanceToFile;
$writememb("G:/Semester 5/Electronic Design Automation/Project1/balanceVerilog.txt", balance);
endtask

task readbalance;
$readmemb("G:/Semester 5/Electronic Design Automation/Project1/balanceVerilog.txt", balance);
endtask
















    always @ (posedge clk or posedge rst)
	begin
		if(rst)
			begin
			current_state <= IDLE;
			end
		else 
			current_state <= next_state;
	end



    always @ (*)
	begin
		if(rst)
        begin
            writeBalanceToFile;
            readall;
			next_state = IDLE;
        end
		else
		begin
		case(current_state)
			IDLE: 
            begin
            i=0;
            DepositDone = 0;
            WithDone =0;
            BalDone = 0;
            foundloc = 4'b1111;
            balancedisplay=0;
            passTrue=0;
            cardTrue=0;
			if(BTN1 == 1 && BTN2==0 && BTN3==0 && (Language==0 || Language ==1)) //BTN1 : Enter Card
				next_state = CardCheck;
			else
				next_state = IDLE;
            end
            CardCheck: begin
            
			if(time_up1 == 1)
				next_state = IDLE; // In case time ended
			else
            begin
            //Loop through cards 3shan n check
            for(i = 0; i < 15; i = i + 1) begin : myfor
                if(cardnumber == card[i] ) begin
                cardTrue = 1;
                foundloc = i;  
                end 
            end
            if(cardTrue == 1)
            begin
                next_state = PASS_ENT_3;
            end
            else
                next_state = IDLE;
            end
            end

			PASS_ENT_3 : 
                begin
                if(pin == password[foundloc]) 
                begin
                   passTrue = 1;
                end

                if(passTrue)
                begin
                    next_state = ATM_MENU;  
                end
                else
                begin
                    next_state = PASS_ENT_2;
                end
                end
			
			PASS_ENT_2 : begin // In case password entered incorrectly ONCE
                if(pin1 == password[foundloc]) 
                   passTrue = 1;
           
                if(passTrue)
                begin
                    next_state = ATM_MENU;
                    
                end
                else
                    next_state = PASS_ENT_1;
            end
			PASS_ENT_1 : // In case password entered incorrectly TWICE
                begin
                if(pin2 == password[foundloc]) 
                   passTrue = 1;
            
                if(passTrue)
                begin
                    next_state = ATM_MENU;
                    
                end
                else
                    next_state = LOCK;
                end
			LOCK: // In case password entered incorrectly 3 TIMES , Lock
            begin
				if(time_up == 1)
					next_state = IDLE; // In case lock time ended
				else
					next_state = LOCK; //in case still locked
            end
			ATM_MENU: // In case password entered correctly
                begin
                if(time_up1 == 1)
				next_state = IDLE; // In case time ended
			    else
                begin
				if(BTN1 == 0 && BTN2 == 0 && BTN3 == 0)  // btn 1 check IDLE, MENU
					next_state = IDLE;					
				else if(BTN1 == 0 && BTN2 == 0 && BTN3 == 1) 
					next_state = Deposit;
				else if(BTN1 == 0 && BTN2 == 1 && BTN3 == 0) 
					next_state = Withdraw;
				else if(BTN1 == 0 && BTN2 == 1 && BTN3 == 1) 
					next_state = Balance;
				else 
					next_state = ATM_MENU;
                end
                end
            Deposit:begin
                if((balance[foundloc] + value) > 4'b1111)
                begin
                    next_state = ATM_MENU;
                end
                else
                begin
                balance[foundloc] = balance[foundloc] + value; 
                
                DepositDone = 1;
                next_state = ATM_MENU;
                end
            end           
            Withdraw:
            begin
                if(balance[foundloc] < value)begin
                  next_state = ATM_MENU;
                end
                else
                begin
                    balance[foundloc] = balance[foundloc] - value; 
                    balancedisplay = balance[foundloc];
                    
                    WithDone =1;
                    next_state = ATM_MENU;
                end
            end
            Balance:
                begin
                balancedisplay = balance[foundloc];
                BalDone = 1; 
                next_state = ATM_MENU;
                end
		endcase
		end
	end



timer timeup(clk,rst,start_timer,time_up);

timer1 timeup1(clk,rst,start_timer1,time_up1);


always @ (posedge clk or posedge rst)
begin 
	if(rst)
		start_timer1 <= 0;
	else
		begin
		if(current_state == IDLE)
			start_timer1 <= 0;
		else
			start_timer1 <= 1;
		end
end


always @ (posedge clk or posedge rst)
begin 
	if(rst)
		begin
		start_timer <= 0;
		end
	else
		begin
		if(current_state == LOCK)
			start_timer <= 1;
		else
			start_timer <= 0;
		end
end


endmodule










