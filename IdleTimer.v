
module timer1(clk, rst, start_timer1, time_up1);

input clk,rst;
input start_timer1;
output reg time_up1;

reg [31:0] counter;

always @(posedge clk or posedge rst) begin
	if(rst) begin
		counter <= 0;
		time_up1 <= 0;
	end
	else begin
		if(start_timer1) begin
			if(counter < 32'd10)
				counter <= counter + 1;
			else
				counter <= counter;
		end
		else begin
			counter <= 0;
		end
		
		time_up1 <= (counter == 32'd10);
		end
	end
endmodule