----------------------------------------------------------------------------------
--!
--! @filename   seven_seg_cntr_pkg.vhd
--! @brief      File holds the package seven_seg_cntr_pkg.vhd
--!
--! @details    seven_seg_cntr_pkg is used to provide several settings for the
--!             seven segment counter.
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

----------------------------------------------------------------------------------
--! @brief     Package to provide procedures and functions to testbench
--!
--! Package contains constants to define which cathode bit is which. This makes it
--! convenient to forward the information in one assignment.
----------------------------------------------------------------------------------
package seven_seg_cntr_pkg is

----------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------

  -- 7-Segment encoding
  constant A_BITNR              : integer := 0;                          --! Bit number for A cathode in std_logic_vector
  constant B_BITNR              : integer := 1;                          --! Bit number for B cathode in std_logic_vector
  constant C_BITNR              : integer := 2;                          --! Bit number for C cathode in std_logic_vector
  constant D_BITNR              : integer := 3;                          --! Bit number for D cathode in std_logic_vector
  constant E_BITNR              : integer := 4;                          --! Bit number for E cathode in std_logic_vector
  constant F_BITNR              : integer := 5;                          --! Bit number for F cathode in std_logic_vector
  constant G_BITNR              : integer := 6;                          --! Bit number for G cathode in std_logic_vector
  constant VECTOR_SIZE          : integer := G_BITNR +1;                 --! Size of vector that will represent the 7-segment display code.

  type seg_out_enc_t is array (0 to 9) of std_logic_vector(VECTOR_SIZE-1 downto 0);
  constant SEG_OUT_ENC          : seg_out_enc_t :=    ("1000000",        -- Encoding for digit 0
                                                       "1111001",        -- Encoding for digit 1
                                                       "0100100",        -- Encoding for digit 2
                                                       "0110000",        -- Encoding for digit 3
                                                       "0011001",        -- Encoding for digit 4
                                                       "0010010",        -- Encoding for digit 5
                                                       "0000010",        -- Encoding for digit 6
                                                       "1111000",        -- Encoding for digit 7
                                                       "0000000",        -- Encoding for digit 8
                                                       "0010000");       -- Encoding for digit 9
  constant SEG_OUT_INVALID      : std_logic_vector(VECTOR_SIZE-1 downto 0) := (others => '1'); -- Don't light up any LEDs for an incorrect input.

  constant NR_OF_SEGMENTS       : integer := 4;                          -- Number of seven-segment elements
  constant SUB_CNT_LEN          : integer := 100000;                     -- Sub counter at 100MHz that is used to switch between segments. Should wrap around 1ms.
----------------------------------------------------------------------------------
-- Type definitions
----------------------------------------------------------------------------------
  type enc_cat_t is array (0 to NR_OF_SEGMENTS-1) of std_logic_vector(VECTOR_SIZE-1 downto 0);

----------------------------------------------------------------------------------
-- Procedure prototypes
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Function prototypes
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Components
----------------------------------------------------------------------------------
  component seven_seg_enc is
    port (
      SYS_CLK_100MHZ              : in  std_logic;
      INT_IN                      : in  std_logic_vector(3 downto 0);
      DOT_IN                      : in  std_logic;
      CAT_OUT                     : out std_logic_vector(VECTOR_SIZE-1 downto 0);
      DP_OUT                      : out std_logic
    );
  end component;

  component seven_seg_mux is
    port (
      RESET                       : in  std_logic;
      SYS_CLK_100MHZ              : in  std_logic;
      ENC_CAT_IN                  : in  enc_cat_t;
      ENC_DP_IN                   : in  std_logic_vector(NR_OF_SEGMENTS-1 downto 0);
      CAT_OUT                     : out std_logic_vector(VECTOR_SIZE-1 downto 0);
      DP_OUT                      : out std_logic;
      AN_OUT                      : out std_logic_vector(NR_OF_SEGMENTS-1 downto 0)
    );
  end component;

  component schmitt_trigger_debounce is
    generic (
      COUNTER_WIDTH               : integer := 7;
      SCHMITT_RISING_THRESHOLD    : integer := 96;
      SCHMITT_FALLING_THRESHOLD   : integer := 32
    );
    port (
      RESET                       : in  std_logic;
      SYS_CLK_100MHZ              : in  std_logic;
      INPUT                       : in  std_logic;
      OUTPUT                      : out std_logic
    );
  end component;

  component bcd_counter is
    port (
      RESET                       : in  std_logic;
      SYS_CLK_100MHZ              : in  std_logic;
      INCR_IN                     : in  std_logic;
      DECR_IN                     : in  std_logic;
      CNT_OUT                     : out std_logic_vector(3 downto 0);
      CARRY_OUT                   : out std_logic;
      DECR_OUT                    : out std_logic
    );
  end component;

end seven_seg_cntr_pkg;

----------------------------------------------------------------------------------
-- Package body
----------------------------------------------------------------------------------
package body seven_seg_cntr_pkg is

----------------------------------------------------------------------------------
-- Procedures
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

end package body;