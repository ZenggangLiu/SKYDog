/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  13/06/26  @  9:45 PM
    FileName: MessageDefines.hpp @ RedSoUL Project
    History:
             - created by: 13/06/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include <cstddef>  /// std::size_t
#include <stdint.h> /// uint32_t,...
/// Library headers
#include "Common/CommonDefines.hpp" /// INLINE_FUNCTION, FOUR_CC
#include "Hashing/RuntimeHash.hpp"  /// RUNTIME_HASH


typedef uint32_t MessageIdValueTypeT;


struct MessageId
{
    /// Message Id的数值
    MessageIdValueTypeT value;

    INLINE_FUNCTION
    bool
    operator== (
        const MessageId rhs) const
    {
        return value == rhs.value;
    }
};


/// std::unordered_map使用的Messsage Id Hash
struct MessageIdHasher
{
    INLINE_FUNCTION
    std::size_t
    operator() (
        const MessageId message_id) const
    {
        /// 计算Hash时使用的32位Seed
        static constexpr uint32_t ID_HASH_SEED = FOUR_CC_32('R', 'M', 'S', 'G');

        return RUNTIME_HASH_64(ID_HASH_SEED, &message_id, sizeof(message_id));
    }
};


/// std::unordered_map使用的Messsage Id比较器
struct MessageIdComparer
{
    /// Compare: if A == B
    INLINE_FUNCTION
    bool
    operator() (
        const MessageId id_a,
        const MessageId id_b) const
    {
        return id_a.value == id_b.value;
    }
};


/// 非法Message Id
#define INVALID_MESSAGE_ID   (MessageId{ (MessageIdValueTypeT)(-1) })

/// 最大的Message长度(字节数)
#define MAXIMAL_MESSAGE_SIZE (256)
