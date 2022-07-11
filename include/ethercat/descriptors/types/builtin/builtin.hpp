/* ============================================================================================================================ *//**
 * @file       builtin.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of members of types describing builtin EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DESCRIPTORS_TYPES_BUILTIN_BUILTIN_H__
#define __ETHERCAT_DESCRIPTORS_TYPES_BUILTIN_BUILTIN_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/descriptors/types/builtin.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors::types {

/* ========================================================= Numeric type ========================================================= */

inline constexpr NumericType NumericType::Bit               { ID::Bit               };
inline constexpr NumericType NumericType::Bool              { ID::Bool              };
inline constexpr NumericType NumericType::Bool8             { ID::Bool8             };
inline constexpr NumericType NumericType::Byte              { ID::Byte              };
inline constexpr NumericType NumericType::Word              { ID::Word              };
inline constexpr NumericType NumericType::DoubleWord        { ID::DoubleWord        };
inline constexpr NumericType NumericType::ShortInt          { ID::ShortInt          };
inline constexpr NumericType NumericType::UnsignedShortInt  { ID::UnsignedShortInt  };
inline constexpr NumericType NumericType::Int               { ID::Int               };
inline constexpr NumericType NumericType::UnsignedInt       { ID::UnsignedInt       };
inline constexpr NumericType NumericType::DoubleInt         { ID::DoubleInt         };
inline constexpr NumericType NumericType::UnsignedDoubleInt { ID::UnsignedDoubleInt };
inline constexpr NumericType NumericType::LongInt           { ID::LongInt           };
inline constexpr NumericType NumericType::UnsignedLongInt   { ID::UnsignedLongInt   };
inline constexpr NumericType NumericType::Real              { ID::Real              };
inline constexpr NumericType NumericType::LongReal          { ID::LongReal          };

inline constexpr NumericType NumericType::BIT               { ID::BIT               };
inline constexpr NumericType NumericType::BOOL              { ID::BOOL              };
inline constexpr NumericType NumericType::BYTE              { ID::BYTE              };
inline constexpr NumericType NumericType::WORD              { ID::WORD              };
inline constexpr NumericType NumericType::DWORD             { ID::DWORD             };
inline constexpr NumericType NumericType::SINT              { ID::SINT              };
inline constexpr NumericType NumericType::USINT             { ID::USINT             };
inline constexpr NumericType NumericType::INT               { ID::INT               };
inline constexpr NumericType NumericType::UINT              { ID::UINT              };
inline constexpr NumericType NumericType::DINT              { ID::DINT              };
inline constexpr NumericType NumericType::UDINT             { ID::UDINT             };
inline constexpr NumericType NumericType::LINT              { ID::LINT              };
inline constexpr NumericType NumericType::ULINT             { ID::ULINT             };
inline constexpr NumericType NumericType::REAL              { ID::REAL              };
inline constexpr NumericType NumericType::LREAL             { ID::LREAL             };

inline constexpr NumericType NumericType::BIT1              { ID::BIT1              };
inline constexpr NumericType NumericType::BOOL1             { ID::BOOL1             };
inline constexpr NumericType NumericType::BOOL8             { ID::BOOL8             };
inline constexpr NumericType NumericType::BYTE8             { ID::BYTE8             };
inline constexpr NumericType NumericType::WORD16            { ID::WORD16            };
inline constexpr NumericType NumericType::DWORD32           { ID::DWORD32           };
inline constexpr NumericType NumericType::SINT8             { ID::SINT8             };
inline constexpr NumericType NumericType::USINT8            { ID::USINT8            };
inline constexpr NumericType NumericType::INT16             { ID::INT16             };
inline constexpr NumericType NumericType::UINT16            { ID::UINT16            };
inline constexpr NumericType NumericType::DINT32            { ID::DINT32            };
inline constexpr NumericType NumericType::UDINT32           { ID::UDINT32           };
inline constexpr NumericType NumericType::LINT64            { ID::LINT64            };
inline constexpr NumericType NumericType::ULINT64           { ID::ULINT64           };
inline constexpr NumericType NumericType::REAL32            { ID::REAL32            };
inline constexpr NumericType NumericType::LREAL64           { ID::LREAL64           };

/* ======================================================== BuiltinTypeBase ======================================================= */

inline constexpr BuiltinType BuiltinType::Bit               { NumericType{ NumericType::ID::Bit               } };
inline constexpr BuiltinType BuiltinType::Bool              { NumericType{ NumericType::ID::Bool              } };
inline constexpr BuiltinType BuiltinType::Bool8             { NumericType{ NumericType::ID::Bool8             } };
inline constexpr BuiltinType BuiltinType::Byte              { NumericType{ NumericType::ID::Byte              } };
inline constexpr BuiltinType BuiltinType::Word              { NumericType{ NumericType::ID::Word              } };
inline constexpr BuiltinType BuiltinType::DoubleWord        { NumericType{ NumericType::ID::DoubleWord        } };
inline constexpr BuiltinType BuiltinType::ShortInt          { NumericType{ NumericType::ID::ShortInt          } };
inline constexpr BuiltinType BuiltinType::UnsignedShortInt  { NumericType{ NumericType::ID::UnsignedShortInt  } };
inline constexpr BuiltinType BuiltinType::Int               { NumericType{ NumericType::ID::Int               } };
inline constexpr BuiltinType BuiltinType::UnsignedInt       { NumericType{ NumericType::ID::UnsignedInt       } };
inline constexpr BuiltinType BuiltinType::DoubleInt         { NumericType{ NumericType::ID::DoubleInt         } };
inline constexpr BuiltinType BuiltinType::UnsignedDoubleInt { NumericType{ NumericType::ID::UnsignedDoubleInt } };
inline constexpr BuiltinType BuiltinType::LongInt           { NumericType{ NumericType::ID::LongInt           } };
inline constexpr BuiltinType BuiltinType::UnsignedLongInt   { NumericType{ NumericType::ID::UnsignedLongInt   } };
inline constexpr BuiltinType BuiltinType::Real              { NumericType{ NumericType::ID::Real              } };
inline constexpr BuiltinType BuiltinType::LongReal          { NumericType{ NumericType::ID::LongReal          } };

inline constexpr BuiltinType BuiltinType::BIT               { NumericType{ NumericType::ID::BIT               } };
inline constexpr BuiltinType BuiltinType::BOOL              { NumericType{ NumericType::ID::BOOL              } };
inline constexpr BuiltinType BuiltinType::BYTE              { NumericType{ NumericType::ID::BYTE              } };
inline constexpr BuiltinType BuiltinType::WORD              { NumericType{ NumericType::ID::WORD              } };
inline constexpr BuiltinType BuiltinType::DWORD             { NumericType{ NumericType::ID::DWORD             } };
inline constexpr BuiltinType BuiltinType::SINT              { NumericType{ NumericType::ID::SINT              } };
inline constexpr BuiltinType BuiltinType::USINT             { NumericType{ NumericType::ID::USINT             } };
inline constexpr BuiltinType BuiltinType::INT               { NumericType{ NumericType::ID::INT               } };
inline constexpr BuiltinType BuiltinType::UINT              { NumericType{ NumericType::ID::UINT              } };
inline constexpr BuiltinType BuiltinType::DINT              { NumericType{ NumericType::ID::DINT              } };
inline constexpr BuiltinType BuiltinType::UDINT             { NumericType{ NumericType::ID::UDINT             } };
inline constexpr BuiltinType BuiltinType::LINT              { NumericType{ NumericType::ID::LINT              } };
inline constexpr BuiltinType BuiltinType::ULINT             { NumericType{ NumericType::ID::ULINT             } };
inline constexpr BuiltinType BuiltinType::REAL              { NumericType{ NumericType::ID::REAL              } };
inline constexpr BuiltinType BuiltinType::LREAL             { NumericType{ NumericType::ID::LREAL             } };

inline constexpr BuiltinType BuiltinType::BIT1              { NumericType{ NumericType::ID::BIT1              } };
inline constexpr BuiltinType BuiltinType::BOOL1             { NumericType{ NumericType::ID::BOOL1             } };
inline constexpr BuiltinType BuiltinType::BOOL8             { NumericType{ NumericType::ID::BOOL8             } };
inline constexpr BuiltinType BuiltinType::BYTE8             { NumericType{ NumericType::ID::BYTE8             } };
inline constexpr BuiltinType BuiltinType::WORD16            { NumericType{ NumericType::ID::WORD16            } };
inline constexpr BuiltinType BuiltinType::DWORD32           { NumericType{ NumericType::ID::DWORD32           } };
inline constexpr BuiltinType BuiltinType::SINT8             { NumericType{ NumericType::ID::SINT8             } };
inline constexpr BuiltinType BuiltinType::USINT8            { NumericType{ NumericType::ID::USINT8            } };
inline constexpr BuiltinType BuiltinType::INT16             { NumericType{ NumericType::ID::INT16             } };
inline constexpr BuiltinType BuiltinType::UINT16            { NumericType{ NumericType::ID::UINT16            } };
inline constexpr BuiltinType BuiltinType::DINT32            { NumericType{ NumericType::ID::DINT32            } };
inline constexpr BuiltinType BuiltinType::UDINT32           { NumericType{ NumericType::ID::UDINT32           } };
inline constexpr BuiltinType BuiltinType::LINT64            { NumericType{ NumericType::ID::LINT64            } };
inline constexpr BuiltinType BuiltinType::ULINT64           { NumericType{ NumericType::ID::ULINT64           } };
inline constexpr BuiltinType BuiltinType::REAL32            { NumericType{ NumericType::ID::REAL32            } };
inline constexpr BuiltinType BuiltinType::LREAL64           { NumericType{ NumericType::ID::LREAL64           } };

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors::types

#endif