#include "GameApp.h"

void Calculate() {
    std::string filename = "test.txt";
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        throw std::runtime_error("无法打开文件");

    // 读取文件内容到缓冲区
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    // 使用MultiByteToWideChar进行编码转换（包含两次调用）
    int requiredSize = MultiByteToWideChar(54936, 0, buffer.data(), static_cast<int>(buffer.size()), NULL, 0);
    if (requiredSize == 0) throw std::runtime_error("转换失败: 需要的空间计算错误");

    std::wstring result(requiredSize, L'\0');
    int convertedSize = MultiByteToWideChar(54936, 0, buffer.data(), static_cast<int>(buffer.size()), &result[0], requiredSize);
    if (convertedSize == 0) throw std::runtime_error("转换失败");

    // 将结果拆分为行
    std::vector<std::wstring> lines;
    std::wistringstream iss(result);
    std::wstring line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    std::string outfile = "out.txt";
    std::ofstream out(outfile);
    if (!out.is_open()) throw std::runtime_error("无法打开输出文件");

    size_t i = 0; // 使用size_t作为索引避免符号问题

    while (i < lines.size()) {
        bool processed = false;

        // 四元数点乘
        if (lines[i] == L"四元数点乘\r") {
            out << "四元数点乘" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            float n0, n1, n2, n3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f %f,%f,%f,%f",
                &m0, &m1, &m2, &m3,
                &n0, &n1, &n2, &n3) != 8) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q1{ m0, m1, m2, m3 }, q2{ n0, n1, n2, n3 };
                out << q1 << "\t" << q2 << "\t" << q1.Dot(q2) << std::endl;
            }
            processed = true;
        }
        // 向量转换欧拉角
        else if (lines[i] == L"向量转换欧拉角\r") {
            out << "向量转换欧拉角" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f",
                &m0, &m1, &m2) != 3) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Vector3f v{ m0, m1, m2 };
                Euler e{ v };
                out << v << "\t" << e.ToDegree() << std::endl;
            }
            processed = true;
        }
        // 向量叉乘
        else if (lines[i] == L"欧拉角转换四元数\r") {
            out << "欧拉角转换四元数" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f",
                &m0, &m1, &m2) != 3) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Euler e{ m0, m1, m2, true };

                out << e.ToDegree() << "\t" << e.ToQuaternion() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"欧拉角转换向量\r") {
            out << "欧拉角转换向量" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f",
                &m0, &m1, &m2) != 3) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Euler e{ m0, m1, m2, true };

                out << e.ToDegree() << "\t" << e.ToVector() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"欧拉角标准化\r") {
            out << "欧拉角标准化" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f",
                &m0, &m1, &m2) != 3) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Euler e{ m0, m1, m2, true };

                out << e.ToDegree() << "\t" << e.Normalize().ToDegree() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数求角度和旋转轴\r") {
            out << "四元数求角度和旋转轴" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2, &m3) != 4) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q{ m0, m1, m2, m3 };

                out << q << "\t" << q.GetAngle(true) << "\t" << q.GetAxis() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数转换矩阵\r") {
            out << "四元数转换矩阵" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2, &m3) != 4) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q{ m0, m1, m2, m3 };

                out << q << "\t" << q.ToRotateMatrix() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数转换欧拉角\r") {
            out << "四元数转换欧拉角" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2, &m3) != 4) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q{ m0, m1, m2, m3 };

                out << q << "\t" << q.ToEuler().ToDegree() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"欧拉角转换矩阵\r") {
            out << "欧拉角转换矩阵" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2) != 3) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Euler e{ m0, m1, m2, true };

                out << e.ToDegree() << "\t" << e.ToRotateMatrix().T() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"矩阵转换欧拉角\r") {
            out << "矩阵转换欧拉角" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15) != 16) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Matrix4f m{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };
                Euler e { m };

                out << m << "\t" << e.ToDegree() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数单位化\r") {
            out << "四元数单位化" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2, &m3) != 4) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q{ m0, m1, m2, m3 };

                out << q << "\t" << q.Normalized() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数插值\r") {
            out << "四元数插值" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            float n0, n1, n2, n3;
            float t;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f %f,%f,%f,%f %f",
                &m0, &m1, &m2, &m3,
                &n0, &n1, &n2, &n3,
                &t) != 9) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q1 { m0, m1, m2, m3 }, q2 { n0, n1, n2, n3 };

                out << q1 << "\t" << q2 << "\t" << t << "\t" << q1.Slerp(q2, t) << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"矩阵转换四元数\r") {
            out << "矩阵转换四元数" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15) != 16) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Matrix4f m{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };
                Quaternion q { m };

                out << m << "\t" << q << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"矩阵正交化\r") {
            out << "矩阵正交化" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15) != 16) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Matrix4f m { m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };

                out << m << "\t" << m.Orthogonalize() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数求逆\r") {
            out << "四元数求逆" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f",
                &m0, &m1, &m2, &m3) != 4) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q{ m0, m1, m2, m3 };

                out << q << "\t" << q.Inversed() << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数求差\r") {
            out << "四元数求差" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            float n0, n1, n2, n3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f %f,%f,%f,%f",
                &m0, &m1, &m2, &m3,
                &n0, &n1, &n2, &n3) != 8) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q1{ m0, m1, m2, m3 };
                Quaternion q2{ n0, n1, n2, n3 };
                out << q1 << "\t" << q2 << "\t" << q1 - q2 << std::endl;
            }
            processed = true;
        }
        else if (lines[i] == L"四元数相乘\r") {
            out << "四元数相乘" << std::endl;
            if (++i >= lines.size()) {
                out << "数据行缺失" << std::endl;
                continue;
            }
            std::wstring dataLine = lines[i];
            float m0, m1, m2, m3;
            float n0, n1, n2, n3;
            if (swscanf_s(dataLine.c_str(),
                L"%f,%f,%f,%f %f,%f,%f,%f",
                &m0, &m1, &m2, &m3,
                &n0, &n1, &n2, &n3) != 8) {
                out << "数据格式错误" << std::endl;
            }
            else {
                Quaternion q1{ m0, m1, m2, m3 };
                Quaternion q2{ n0, n1, n2, n3 };
                out << q1 << "\t" << q2 << "\t" << q1 * q2 << std::endl;
            }
            processed = true;
            }

        if (processed) i += 1;
        else ++i;   // 未处理则跳转一行
    }

    out.close();
}
