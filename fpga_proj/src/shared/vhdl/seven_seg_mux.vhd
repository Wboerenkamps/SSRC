----------------------------------------------------------------------------------
--!
--! @filename   seven_seg_mux.vhd
--! @brief      File holds the entity seven_seg_mux
--!
--! @details    seven_seg_mux is used to multiplex one encoder to multiple
--!             7-segments. This muxing is required to drive all the 7-segments
--!             on the Basys3 board separately.
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

library ieee;
use ieee.std_logic_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use ieee.numeric_std.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library unisim;
--use unisim.vcomponents.all;

library work;
use work.seven_seg_cntr_pkg.all;


----------------------------------------------------------------------------------
--! @brief     seven_seg_mux is used to multiplex all encoders into the 7-segment elements.
--!
--! seven_seg_mux receives the encoded signals from all encoders, and time-domain multiplexes
--! these to 7-segments.
----------------------------------------------------------------------------------
entity seven_seg_mux is
  --generic (
  --);
  port (
    RESET                       : in  std_logic;                                      --! Active high reset signal to reset seven_seg_mux
    SYS_CLK_100MHZ              : in  std_logic;                                      --! System clock running at 100 MHz
    -- Input signals from encoders
    ENC_CAT_IN                  : in  enc_cat_t;
    ENC_DP_IN                   : in  std_logic_vector(NR_OF_SEGMENTS-1 downto 0);
    -- 7-Segment output signals
    CAT_OUT                     : out std_logic_vector(VECTOR_SIZE-1 downto 0);       --! Cathode outputs
    DP_OUT                      : out std_logic;
    AN_OUT                      : out std_logic_vector(NR_OF_SEGMENTS-1 downto 0)
  );
end seven_seg_mux;

----------------------------------------------------------------------------------
-- Architecture
----------------------------------------------------------------------------------
architecture behavioral of seven_seg_mux is

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
signal seg_cnt                : integer range 0 to NR_OF_SEGMENTS -1 := 0;
signal sub_cnt                : integer range 0 to SUB_CNT_LEN-1;
signal sub_cnt_wrap           : std_logic;

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
  --! @brief    sub_cnt_mux_proc is used to implement a "subsampling" counter
  --!
  --! This process is used to implement a counter that is used to reduce the frequency
  --! at which the switching between the different 7-segments occurs.
  ----------------------------------------------------------------------------------
  sub_cnt_mux_proc: process(SYS_CLK_100MHZ) is
  begin
    if rising_edge(SYS_CLK_100MHZ) then
      sub_cnt_wrap      <= '0';
      if RESET = '1' then
        sub_cnt         <= 0;
      else
        if (sub_cnt < SUB_CNT_LEN -1) then
          sub_cnt       <= sub_cnt +1;
        else
          sub_cnt       <= 0;
          sub_cnt_wrap  <= '1';
        end if;
      end if;
    end if;
  end process;

  ----------------------------------------------------------------------------------
  --! @brief    seg_muxing_proc is used to implement the actual time-domain multiplexing
  --!
  --! This process is used to implement time-domain multiplexing access to several
  --! 7-segment elements.
  ----------------------------------------------------------------------------------
  seg_muxing_proc: process(SYS_CLK_100MHZ) is
  begin
    if rising_edge(SYS_CLK_100MHZ) then
      -- Default all segments off
      AN_OUT            <= (others => '1');
      -- Implement a segment counter with reset
      if RESET = '1' then
        seg_cnt         <= 0;
      else
        if (sub_cnt_wrap = '1') then
          if (seg_cnt < NR_OF_SEGMENTS-1) then
            seg_cnt     <= seg_cnt +1;
          else
            seg_cnt     <= 0;
          end if;
        end if;
        -- Tie anode to GND that corresponds to the segment counter.
        AN_OUT(seg_cnt) <= '0';
        -- Tie the proper cathodes to the segment that corresponds to the counter state.
        CAT_OUT         <= ENC_CAT_IN(seg_cnt);
        -- Tie the decimal point to the segment that corresponds to the counter state.
        DP_OUT          <= ENC_DP_IN(seg_cnt);
      end if;
    end if;
  end process;

end behavioral;