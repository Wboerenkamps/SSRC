----------------------------------------------------------------------------------
--!
--! @filename   schmitt_trigger_debounce.vhd
--! @brief      File holds the entity schmitt_trigger_debounce
--!
--! @details    schmitt_trigger_debounce is used to debounce inputs in a Schmitt-trigger
--!             alike way. When in a certain state, it tends to stay in that state, unless
--!             the other state is active for a time that is long enough.
--!
--! @copyright  (c) Core|Vision B.V.,
--!             Cereslaan 10b,
--!             5384 VT Heesch,
--!             The Netherlands.
--!             All Rights Reserved.
--!
--! @customer   Avans 's Hertogenbosch
--! @project    Security workshop
--! @module     seven_seg_cntr
--!
--! @author     Jurgen Gerritsen
--! @svnversion $Date$
--!             $Revision$
--!
----------------------------------------------------------------------------------


library IEEE;
  use IEEE.STD_LOGIC_1164.ALL;
  use IEEE.NUMERIC_STD.ALL;
  use IEEE.STD_LOGIC_MISC.ALL;



----------------------------------------------------------------------------------
-- Entity
----------------------------------------------------------------------------------
entity schmitt_trigger_debounce is
  generic (
    COUNTER_WIDTH               : integer := 7;      -- Width of schmitt trigger counter in number of bits
    SCHMITT_RISING_THRESHOLD    : integer := 96;     -- Counter value at or above which the output asserts to '1'; e.g 3/4 of 2**COUNTER_WIDTH
    SCHMITT_FALLING_THRESHOLD   : integer := 32      -- Counter value below which the output asserts to '0'; e.g 1/4 of 2**COUNTER_WIDTH
  );
  port (
    RESET                       : in  std_logic;     -- Resets counter and resets output to '0'
    SYS_CLK_100MHZ              : in  std_logic;     -- System clock of 100MHz
    INPUT                       : in  std_logic;     -- Schmitt trigger input
    OUTPUT                      : out std_logic      -- Schmitt trigger-ed output
  );
end schmitt_trigger_debounce;
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Architecture
----------------------------------------------------------------------------------
architecture Behavioral of schmitt_trigger_debounce is

----------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Type definitions
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Component declarations
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Signal declarations
----------------------------------------------------------------------------------
signal schmitt_trigger_counter              : unsigned(COUNTER_WIDTH-1 downto 0) := (others => '0');
signal out_reg                              : std_logic := '0';

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Procedures
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Attributes
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Functionality
----------------------------------------------------------------------------------
begin
  ----------------------------------------------------------------------------------
  --! @brief    schmitt_trigger_debounce_proc is used to implement "Schmitt-trigger" debouncer
  --!
  --! This process is used to implement a "Schmitt-trigger" debouncing component.
  --! The component uses a counter to count the difference between the amount of '1's
  --! and '0's. When the number of '1's is above a certain threshold, the output will
  --! also become '1'. Then, in order to have an output of '0', the number of '0's
  --! at the input will need to be at least the falling threshold before the output
  --! goes to '0'. This is more-or-less like a Schmitt-trigger component behaves like for
  --! voltages.
  ----------------------------------------------------------------------------------
  schmitt_trigger_debounce_proc : process(SYS_CLK_100MHZ)
  begin
    if rising_edge(SYS_CLK_100MHZ) then
      -- Up/down counter
      if RESET = '1' then
        schmitt_trigger_counter <= (others => '0');
        out_reg                 <= '0';
      else
        if INPUT = '1' then
          -- count up
          if schmitt_trigger_counter < 2**COUNTER_WIDTH then
            schmitt_trigger_counter <= schmitt_trigger_counter + 1;
          end if;
          -- Swap/make output active when the rising threshold is reached.
          if schmitt_trigger_counter >= to_unsigned(SCHMITT_RISING_THRESHOLD,  schmitt_trigger_counter'length) then
            if out_reg = '0' then
              schmitt_trigger_counter <= (others => '1'); -- latch to max count at output change
            end if;
            out_reg                   <= '1';
          end if;
        elsif INPUT = '0' then
          -- count down
          if schmitt_trigger_counter > 0 then
            schmitt_trigger_counter <= schmitt_trigger_counter - 1;
          end if;
          -- Swap/make output inactive when the falling threshold is reached.
          if schmitt_trigger_counter < to_unsigned(SCHMITT_FALLING_THRESHOLD,  schmitt_trigger_counter'length) then
            if out_reg = '1' then
              schmitt_trigger_counter <= (others => '0'); -- latch to min count at output change
            end if;
            out_reg                   <= '0';
          end if;
        end if;
      end if;
    end if;
  end process;

  OUTPUT <= out_reg;

end Behavioral;
