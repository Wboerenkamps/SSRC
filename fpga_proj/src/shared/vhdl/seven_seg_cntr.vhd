----------------------------------------------------------------------------------
--!
--! @filename   seven_seg_cntr.vhd
--! @brief      File holds the entity seven_seg_cntr
--!
--! @details    seven_seg_cntr is used to implement a BCD up/down counter that displays its
--!             value on a 7-segment display.
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
--! @brief     Entity is used to implement the 7-segment display up/down counter
--!
--! The seven_seg_cntr entity is used to implement the complete 7-segment display up/down
--! counter using a BCD counter and a 7-segment encoder
----------------------------------------------------------------------------------
entity seven_seg_cntr is
  --generic (
  --);
  port (
    RESET                       : in  std_logic;                                      --! Active high reset signal to reset seven_seg_cntr
    SYS_CLK_100MHZ              : in  std_logic;                                      --! System clock running at 100 MHz
    -- Input signals
    UP_BTN                      : in  std_logic;                                      --! Button for the up signal
    DOWN_BTN                    : in  std_logic;                                      --! Button for the down signal
    -- 7-Segment signals
    AN0                         : out std_logic;                                      --! Anode output for 7-segment module 0
    AN1                         : out std_logic;                                      --! Anode output for 7-segment module 1
    AN2                         : out std_logic;                                      --! Anode output for 7-segment module 2
    AN3                         : out std_logic;                                      --! Anode output for 7-segment module 3
    CA                          : out std_logic;                                      --! Top middle cathode
    CB                          : out std_logic;                                      --! Top right cathode
    CC                          : out std_logic;                                      --! Bottom right cathode
    CD                          : out std_logic;                                      --! Bottom middle cathode
    CE                          : out std_logic;                                      --! Bottom left cathode
    CF                          : out std_logic;                                      --! Top left cathode
    CG                          : out std_logic;                                      --! Absolute middle cathode
    DP                          : out std_logic                                       --! Dot Point cathode
  );
end seven_seg_cntr;

----------------------------------------------------------------------------------
-- Architecture
----------------------------------------------------------------------------------
architecture behavioral of seven_seg_cntr is

----------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Type definitions
----------------------------------------------------------------------------------
type bcd_counter_val_t is array (0 to NR_OF_SEGMENTS-1) of std_logic_vector(3 downto 0);

----------------------------------------------------------------------------------
-- Component declarations
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Signal declarations
----------------------------------------------------------------------------------
signal enc_dot_in             : std_logic_vector(NR_OF_SEGMENTS-1 downto 0);
signal enc_cat_out            : enc_cat_t;
signal enc_dp_out             : std_logic_vector(NR_OF_SEGMENTS-1 downto 0);
signal cat_vec                : std_logic_vector(VECTOR_SIZE-1 downto 0);
signal an_vec                 : std_logic_vector(NR_OF_SEGMENTS-1 downto 0);
signal dp_i                   : std_logic;
signal up_btn_debounced       : std_logic;
signal down_btn_debounced     : std_logic;
signal up_btn_debounced_prev  : std_logic;
signal down_btn_debounced_prev: std_logic;
signal up_btn_pulse           : std_logic;
signal down_btn_pulse         : std_logic;
signal bcd_counter_incr       : std_logic_vector(NR_OF_SEGMENTS downto 0);
signal bcd_counter_decr       : std_logic_vector(NR_OF_SEGMENTS downto 0);
signal bcd_counter_val        : bcd_counter_val_t;

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

  enc_dot_in(0) <= '1';
  enc_dot_in(1) <= '1';
  enc_dot_in(2) <= '1';
  enc_dot_in(3) <= '1';

  debounce_up_btn: schmitt_trigger_debounce
    generic map(
      COUNTER_WIDTH               => 7,
      SCHMITT_RISING_THRESHOLD    => 96,
      SCHMITT_FALLING_THRESHOLD   => 32
    )
    port map(
      RESET                       => RESET,
      SYS_CLK_100MHZ              => SYS_CLK_100MHZ,
      INPUT                       => UP_BTN,
      OUTPUT                      => up_btn_debounced
    );

  debounce_down_btn: schmitt_trigger_debounce
    generic map(
      COUNTER_WIDTH               => 7,
      SCHMITT_RISING_THRESHOLD    => 96,
      SCHMITT_FALLING_THRESHOLD   => 32
    )
    port map(
      RESET                       => RESET,
      SYS_CLK_100MHZ              => SYS_CLK_100MHZ,
      INPUT                       => DOWN_BTN,
      OUTPUT                      => down_btn_debounced
    );

  bcd_counter_incr(0)          <= up_btn_pulse;
  bcd_counter_decr(0)          <= down_btn_pulse;
  bcd_counter_gen: for i in 0 to NR_OF_SEGMENTS-1 generate
    bcd_counter_i: bcd_counter
      port map(
        RESET                       => RESET,
        SYS_CLK_100MHZ              => SYS_CLK_100MHZ,
        INCR_IN                     => bcd_counter_incr(i),
        DECR_IN                     => bcd_counter_decr(i),
        CNT_OUT                     => bcd_counter_val(i),
        CARRY_OUT                   => bcd_counter_incr(i+1),
        DECR_OUT                    => bcd_counter_decr(i+1)
      );
  end generate;

  button_to_pulse: process(SYS_CLK_100MHZ) is
    begin
      if rising_edge(SYS_CLK_100MHZ) then
        up_btn_pulse            <= '0';
        down_btn_pulse          <= '0';
        up_btn_debounced_prev   <= up_btn_debounced;
        down_btn_debounced_prev <= down_btn_debounced;
        if up_btn_debounced_prev = '0' and up_btn_debounced = '1' then
          up_btn_pulse          <= '1';
        elsif down_btn_debounced_prev = '0' and down_btn_debounced = '1' then
          down_btn_pulse        <= '1';
        end if;
      end if;
    end process;


  ----------------------------------------------------------------------------------
  --! @brief    seven_seg_encn is used to implement a 7-segment encoder for module n
  --!
  --! The seven_seg_encn component is used to implement a 7-segment encoder that is used to
  --! drive the 7-segment module n.
  ----------------------------------------------------------------------------------
  seven_seg_enc_loop: for i in 0 to NR_OF_SEGMENTS-1 generate
    seven_seg_encn: seven_seg_enc
      --generic map(
      --)
      port map(
        SYS_CLK_100MHZ            => SYS_CLK_100MHZ,
        INT_IN                    => bcd_counter_val(i),
        DOT_IN                    => enc_dot_in(i),
        CAT_OUT                   => enc_cat_out(i),
        DP_OUT                    => enc_dp_out(i)
      );
  end generate;

  ----------------------------------------------------------------------------------
  --! @brief    seven_seg_mux is used to implement a 7-segment time-domain multiplexer
  --!
  --! The seven_seg_mux is used to connect all 7-segment encoders to the proper 7-segments
  --! using time-domain multiplexing. This is required to drive all the 7-segments that
  --! are present on the Basys3 board.
  ----------------------------------------------------------------------------------
  seven_seg_mux_i: seven_seg_mux
    --generic map(
    --)
    port map(
      RESET                       => RESET,
      SYS_CLK_100MHZ              => SYS_CLK_100MHZ,
      ENC_CAT_IN                  => enc_cat_out,
      ENC_DP_IN                   => enc_dp_out,
      CAT_OUT                     => cat_vec,
      DP_OUT                      => dp_i,
      AN_OUT                      => an_vec
    );


  -- Map proper bits to cathode outputs.
  CA <= cat_vec(A_BITNR);
  CB <= cat_vec(B_BITNR);
  CC <= cat_vec(C_BITNR);
  CD <= cat_vec(D_BITNR);
  CE <= cat_vec(E_BITNR);
  CF <= cat_vec(F_BITNR);
  CG <= cat_vec(G_BITNR);
  DP <= dp_i;

  -- Map proper bits to anode outputs.
  AN0 <= an_vec(0);
  AN1 <= an_vec(1);
  AN2 <= an_vec(2);
  AN3 <= an_vec(3);

end behavioral;