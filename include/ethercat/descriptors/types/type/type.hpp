/* ============================================================================================================================ *//**
 * @file       type.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 8th June 2022 6:23:13 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definitions of members of types describing EtherCAT (CoE) types
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_DESCRIPTORS_TYPES_TYPE_TYPE_H__
#define __ETHERCAT_DESCRIPTORS_TYPES_TYPE_TYPE_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/descriptors/types/type.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::descriptors::types {

/* ============================================================= Type ============================================================= */

template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Bit               { BuiltinType{ NumericType{ NumericType::ID::Bit               } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Bool              { BuiltinType{ NumericType{ NumericType::ID::Bool              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Bool8             { BuiltinType{ NumericType{ NumericType::ID::Bool8             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Byte              { BuiltinType{ NumericType{ NumericType::ID::Byte              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Word              { BuiltinType{ NumericType{ NumericType::ID::Word              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DoubleWord        { BuiltinType{ NumericType{ NumericType::ID::DoubleWord        } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::ShortInt          { BuiltinType{ NumericType{ NumericType::ID::ShortInt          } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UnsignedShortInt  { BuiltinType{ NumericType{ NumericType::ID::UnsignedShortInt  } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Int               { BuiltinType{ NumericType{ NumericType::ID::Int               } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UnsignedInt       { BuiltinType{ NumericType{ NumericType::ID::UnsignedInt       } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DoubleInt         { BuiltinType{ NumericType{ NumericType::ID::DoubleInt         } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UnsignedDoubleInt { BuiltinType{ NumericType{ NumericType::ID::UnsignedDoubleInt } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LongInt           { BuiltinType{ NumericType{ NumericType::ID::LongInt           } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UnsignedLongInt   { BuiltinType{ NumericType{ NumericType::ID::UnsignedLongInt   } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::Real              { BuiltinType{ NumericType{ NumericType::ID::Real              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LongReal          { BuiltinType{ NumericType{ NumericType::ID::LongReal          } } };

template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BIT               { BuiltinType{ NumericType{ NumericType::ID::BIT               } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BOOL              { BuiltinType{ NumericType{ NumericType::ID::BOOL              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BYTE              { BuiltinType{ NumericType{ NumericType::ID::BYTE              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::WORD              { BuiltinType{ NumericType{ NumericType::ID::WORD              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DWORD             { BuiltinType{ NumericType{ NumericType::ID::DWORD             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::SINT              { BuiltinType{ NumericType{ NumericType::ID::SINT              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::USINT             { BuiltinType{ NumericType{ NumericType::ID::USINT             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::INT               { BuiltinType{ NumericType{ NumericType::ID::INT               } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UINT              { BuiltinType{ NumericType{ NumericType::ID::UINT              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DINT              { BuiltinType{ NumericType{ NumericType::ID::DINT              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UDINT             { BuiltinType{ NumericType{ NumericType::ID::UDINT             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LINT              { BuiltinType{ NumericType{ NumericType::ID::LINT              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::ULINT             { BuiltinType{ NumericType{ NumericType::ID::ULINT             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::REAL              { BuiltinType{ NumericType{ NumericType::ID::REAL              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LREAL             { BuiltinType{ NumericType{ NumericType::ID::LREAL             } } };

template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BIT1              { BuiltinType{ NumericType{ NumericType::ID::BIT1              } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BOOL1             { BuiltinType{ NumericType{ NumericType::ID::BOOL1             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BOOL8             { BuiltinType{ NumericType{ NumericType::ID::BOOL8             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::BYTE8             { BuiltinType{ NumericType{ NumericType::ID::BYTE8             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::WORD16            { BuiltinType{ NumericType{ NumericType::ID::WORD16            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DWORD32           { BuiltinType{ NumericType{ NumericType::ID::DWORD32           } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::SINT8             { BuiltinType{ NumericType{ NumericType::ID::SINT8             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::USINT8            { BuiltinType{ NumericType{ NumericType::ID::USINT8            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::INT16             { BuiltinType{ NumericType{ NumericType::ID::INT16             } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UINT16            { BuiltinType{ NumericType{ NumericType::ID::UINT16            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::DINT32            { BuiltinType{ NumericType{ NumericType::ID::DINT32            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::UDINT32           { BuiltinType{ NumericType{ NumericType::ID::UDINT32           } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LINT64            { BuiltinType{ NumericType{ NumericType::ID::LINT64            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::ULINT64           { BuiltinType{ NumericType{ NumericType::ID::ULINT64           } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::REAL32            { BuiltinType{ NumericType{ NumericType::ID::REAL32            } } };
template<std::size_t SubitemsN> inline constexpr Type<SubitemsN> Type<SubitemsN>::LREAL64           { BuiltinType{ NumericType{ NumericType::ID::LREAL64           } } };

/* ================================================================================================================================ */

} // End namespace ethercat::descriptors::types

#endif