

module timer(clk, rst, start_timer, time_up);

input clk,rst;
input start_timer;
output reg time_up;

reg [31:0] counter;

always @(posedge clk or posedge rst) begin
	if(rst) begin
		counter <= 0;
		time_up <= 0;
	end
	else begin
		if(start_timer) begin
			if(counter < 32'd1)
				counter <= counter + 1;
			else
				counter <= counter;
		end
		else begin
			counter <= 0;
		end
		
		time_up <= (counter == 32'd1);
		end
	end
endmodule