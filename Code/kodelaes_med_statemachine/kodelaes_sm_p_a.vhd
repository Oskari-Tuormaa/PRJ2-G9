library ieee;
use ieee.std_logic_1164.all;
use work.all;
use ieee.numeric_std.all;

entity kodelaes_sm_p_a is
port
		(
		CLOCK_50: in std_logic;
		KEY: in std_logic_vector(3 downto 2);
		SW: in std_logic_vector(3 downto 0);
		GPIO_0: out std_logic_vector(0 downto 0);
		LEDG: out std_logic_vector (0 downto 0);
		LEDR: out std_logic_vector(5 downto 0)
		);
end;

architecture structural of kodelaes_sm_p_a is
begin
	U1: entity kodelaes_sm port map
	(clk => CLOCK_50, reset => KEY(2), enter => KEY(3), code => SW, lock => GPIO_0(0), LED => LEDR, aaben => LEDG(0));
end;
