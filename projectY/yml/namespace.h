#ifndef YML_NAMESPACE_H
#define YML_NAMESPACE_H

#ifndef YML_NAMESPACE_BEGIN
    ///A macro that can be overwritten
    ///by the user if the yml namespace
    ///conflicts with a current namespace
    ///in their code base
#   define YML_NAMESPACE_BEGIN namespace yml {
#endif //YML_NAMESPACE_BEGIN

#ifndef YML_NAMESPACE_END
    ///A macro that can be overwritten
    ///by the user if the yml namespace
    ///conflicts with a current namespace
    ///in their code base
#   define YML_NAMESPACE_END }
#endif //YML_NAMESPACE_END

#endif //YML_NAMESPACE_H
