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
    Created:  16/05/26  @  6:50 PM
    FileName: TransformMarker.hpp @ RedSoUL Project
    History:
             - created by: 16/05/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// Library headers
#include "SceneGraph/ObjectMarker.hpp"


class TransformMarker;


struct TransformCreateParam : CreateParameter
{
    typedef CreateParameter SuperT;

    /// 父节点的空间变换属性
    /// NOTE: CAN BE NULL
    TransformMarker * father;

    TransformCreateParam (
        const StaticStringIdT   name_id,
        SceneObject &           owner,
        TransformMarker * const father);
};


/// 空间变换属性(维护SceneObject层级关系的属性)
class TransformMarker : public ObjectMarker
{
public:


private:
    friend class SceneObject;

    typedef ObjectMarker SuperT;

    /// 创建函数
    static
    ObjectMarker *
    create (
        const CreateParameter & parameter);

    /// 销毁函数
    ///
    /// @param[in,out] marker
    ///     属性实例。设置为nullptr, 如果销毁成功
    /// @return
    ///     True,  如果销毁成功
    ///     False, 如果销毁失败
    static
    bool
    destroy (
        ObjectMarker * & marker);

    TransformMarker (
        SceneObject &           owner,
        TransformMarker * const father);

    ~TransformMarker ();

    TransformMarker (
        const TransformMarker &) = delete;
    TransformMarker & operator = (
        const TransformMarker &) = delete;

private:
    /// 属性类型信息
    static const MarkerTypeInfo ms_type_info;
};
