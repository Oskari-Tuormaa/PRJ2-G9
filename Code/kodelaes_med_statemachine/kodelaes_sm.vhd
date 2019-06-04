library ieee;
use ieee.std_logic_1164.all;
use work.all;
use ieee.numeric_std.all;

entity kodelaes_sm is
port 
		(
		reset,enter,clk: in std_logic;
		code: in std_logic_vector(3 downto 0);
		lock: out std_logic;
		err_event, failed: buffer std_logic;
		aaben : out std_logic;
		LED : out std_logic_vector(5 downto 0)
		);
end;

architecture three_processes of kodelaes_sm is
	type state is 
	(going_idle, idle, evaluating_code_1, 
	getting_code_2, evaluating_code_2, unlocked,
	wrong_code, blink_1, blink_2, blink_3, blink_4, blink_5, blink_6, permanently_locked);
	signal present_state, next_state: state;
	
	subtype state2 is std_logic_vector(1 downto 0);
	constant Err_0: state2 := "00";
	constant Err_1: state2 := "01";
	constant Err_2: state2 := "10";
	constant Err_3: state2 := "11";
	signal present_state2, next_state2: state2;
	
	signal counter: integer :=0;
	signal switch : std_logic;
begin
	
state_reg: process(clk, reset)		--låst og ikke låst states
begin
	if reset = '0' then
		present_state <= idle;
	elsif rising_edge(clk) then
		present_state <= next_state;
	end if;
end process;	

state_reg2: process(clk, reset)		--forkert kode counter
begin
	if reset = '0' then
		present_state2 <= Err_0;
	elsif rising_edge(clk) then
		present_state2 <= next_state2;
	end if;
end process;	

outputs: process (present_state, enter)
begin
	aaben <= '0';
	lock <= '0';
	
case present_state is
	
when idle => 
	LED <= "000001";
	
when evaluating_code_1 => 
	LED <= "000001";
		
when getting_code_2 => 
	LED <= "000010";
		
when evaluating_code_2 => 
	LED <= "000010";
		
when unlocked => 
	lock <= '1';
	aaben <= '1';
	LED <= "000000";
	
when going_idle => 
	LED <= "000000";
	
when wrong_code =>
	LED <= "011100";

when blink_1 =>
	LED <= "000000";

when blink_2 =>
	LED <= "011100";
	
when blink_3 =>
	LED <= "000000";

when blink_4 =>
	LED <= "011100";

when blink_5 =>
	LED <= "000000";

when blink_6 =>
	LED <= "011100";
	
when permanently_locked => 
	LED <= "111111";
		
end case;

end process;



outputs2: process (present_state2, err_event)
begin

case present_state2 is
	
when Err_0 => failed <= '0';
		
when Err_1 => failed <= '0';
		
when Err_2 => failed <= '0';
		
when Err_3 => failed <= '1';

end case;

end process;


nxt_state: process(present_state, enter, clk)
variable code1, code2: std_logic;
begin

	next_state <= present_state;
	err_event <= '0';
	
case present_state is
when idle =>
	if enter = '0' then
		next_state <= evaluating_code_1;
	end if;
		
when evaluating_code_1 =>
	if enter = '1' then
		if code = "0101" then
			next_state <= getting_code_2;
		else 
			next_state <= wrong_code;
			err_event <= '1';
		end if;
	end if;
		
when getting_code_2 =>
	if enter = '0' then
		next_state <= evaluating_code_2;
	end if;
			
when evaluating_code_2 =>
	if enter = '1' then
		if code = "1010" then
			next_state <= unlocked;
		else 
			next_state <= wrong_code;
			err_event <= '1';
		end if;
	end if;
			
when wrong_code =>
if switch = '1' then
 next_state <= blink_1;
end if;

when blink_1 =>
if switch = '0' then
 next_state <= blink_2;
end if;

when blink_2 =>
if switch = '1' then
 next_state <= blink_3;
end if;

when blink_3 =>
if switch = '0' then
 next_state <= blink_4;
end if;

when blink_4 =>
if switch = '1' then
 next_state <= blink_5;
end if;

when blink_5 =>
if switch = '0' then
 next_state <= blink_6;
end if;

when blink_6 =>
if switch = '1' then
	if failed = '1' then
		next_state <= permanently_locked;
	else
		next_state <= idle;
	end if;
end if;
	
when permanently_locked =>
	next_state <= permanently_locked;
			
when unlocked => 
	if enter = '0' then
		next_state <= going_idle;
	end if;
			
when going_idle =>
	if enter = '1' then
		next_state <= idle;
	end if;


end case;
end process;

conter: process (clk)
begin 

if rising_edge(clk) then

	counter <= counter +1;
	
if present_state = evaluating_code_1 then
	counter <= 0;
end if;
if present_state = evaluating_code_2 then
	counter <= 0;
end if;
	if counter = 10000000 then -- ændre her for at ændre blinke hastihed
		counter <= 0;
		if switch = '1' then
			switch <= '0';
		else
			switch <= '1';
		end if;
	end if;
end if;

end process;


nxt_state2: process(present_state2, err_event)
begin 

next_state2 <= present_state2;

	case present_state2 is
		when Err_0 =>
		if present_state = unlocked then 
		next_state2 <= err_0;
		elsif err_event = '1' then
			next_state2 <= Err_1;
		end if;
		
		when Err_1 =>
		if present_state = unlocked then 
		next_state2 <= err_0;
		elsif err_event = '1' then
			next_state2 <= Err_2;
		end if;
		
		when Err_2 =>
		if present_state = unlocked then 
		next_state2 <= err_0;
		elsif err_event = '1' then
			next_state2 <= Err_3;
		end if;
			
		when Err_3 =>
		if err_event = '1' then
			next_state2 <= Err_3;
		end if;
		
	end case;
end process;

end;
	