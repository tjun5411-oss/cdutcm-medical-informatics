<?php
/**
 * 随机密码生成器
 * 功能：根据用户指定的长度和字符类型，生成安全的随机密码
 * 作者：PHP Demo
 * 版本：1.0
 */

// 初始化变量
$generated_password = '';
$error_message = '';
$length = 12; // 默认长度
$use_uppercase = true;
$use_lowercase = true;
$use_numbers = true;
$use_symbols = false;

// 处理表单提交
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // 获取并验证长度
    if (isset($_POST['length']) && is_numeric($_POST['length'])) {
        $length = intval($_POST['length']);
        if ($length < 4) {
            $error_message = '密码长度不能小于4位！';
            $length = 4;
        } elseif ($length > 64) {
            $error_message = '密码长度不能大于64位！';
            $length = 64;
        }
    } else {
        $length = 12;
    }

    // 获取字符类型选项
    $use_uppercase = isset($_POST['uppercase']);
    $use_lowercase = isset($_POST['lowercase']);
    $use_numbers = isset($_POST['numbers']);
    $use_symbols = isset($_POST['symbols']);

    // 至少选择一种字符类型
    if (!$use_uppercase && !$use_lowercase && !$use_numbers && !$use_symbols) {
        $error_message = '请至少选择一种字符类型！';
    } else {
        // 生成密码
        $generated_password = generatePassword($length, $use_uppercase, $use_lowercase, $use_numbers, $use_symbols);
        if ($generated_password === false) {
            $error_message = '密码生成失败，请稍后重试。';
        }
    }
}

/**
 * 生成随机密码函数
 * @param int $length 密码长度
 * @param bool $uppercase 是否包含大写字母
 * @param bool $lowercase 是否包含小写字母
 * @param bool $numbers 是否包含数字
 * @param bool $symbols 是否包含特殊符号
 * @return string|false 返回密码字符串或false
 */
function generatePassword($length, $uppercase, $lowercase, $numbers, $symbols) {
    $char_pool = '';
    if ($uppercase) $char_pool .= 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    if ($lowercase) $char_pool .= 'abcdefghijklmnopqrstuvwxyz';
    if ($numbers) $char_pool .= '0123456789';
    if ($symbols) $char_pool .= '!@#$%^&*()_+-=[]{}|;:,.<>?';

    $pool_size = strlen($char_pool);
    if ($pool_size == 0) return false;

    // 使用随机性更强的 random_int 生成密码
    $password = '';
    for ($i = 0; $i < $length; $i++) {
        $random_index = random_int(0, $pool_size - 1);
        $password .= $char_pool[$random_index];
    }
    return $password;
}

// 计算密码强度（简单演示）
function getPasswordStrength($password) {
    $score = 0;
    if (preg_match('/[A-Z]/', $password)) $score++;
    if (preg_match('/[a-z]/', $password)) $score++;
    if (preg_match('/[0-9]/', $password)) $score++;
    if (preg_match('/[^A-Za-z0-9]/', $password)) $score++;
    if (strlen($password) >= 12) $score++;

    switch ($score) {
        case 0:
        case 1:
            return '非常弱';
        case 2:
            return '弱';
        case 3:
            return '中等';
        case 4:
            return '强';
        case 5:
            return '非常强';
        default:
            return '未知';
    }
}
?>
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>随机密码生成器 - PHP工具</title>
    <style>
        * {
            box-sizing: border-box;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.2);
            padding: 30px;
            max-width: 550px;
            width: 100%;
            transition: transform 0.3s ease;
        }
        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 10px;
            font-size: 28px;
        }
        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 14px;
            border-bottom: 1px solid #eee;
            padding-bottom: 15px;
        }
        .form-group {
            margin-bottom: 20px;
        }
        label {
            display: block;
            margin-bottom: 8px;
            font-weight: 600;
            color: #555;
        }
        input[type="number"] {
            width: 100%;
            padding: 12px;
            border: 1px solid #ddd;
            border-radius: 8px;
            font-size: 16px;
            transition: border 0.3s;
        }
        input[type="number"]:focus {
            outline: none;
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102,126,234,0.1);
        }
        .checkbox-group {
            display: flex;
            flex-wrap: wrap;
            gap: 15px;
            margin-top: 5px;
        }
        .checkbox-group label {
            display: flex;
            align-items: center;
            font-weight: normal;
            cursor: pointer;
            gap: 6px;
        }
        button {
            width: 100%;
            padding: 14px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 18px;
            font-weight: bold;
            cursor: pointer;
            transition: transform 0.1s, box-shadow 0.2s;
        }
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 7px 14px rgba(0,0,0,0.2);
        }
        button:active {
            transform: translateY(1px);
        }
        .result {
            margin-top: 30px;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 12px;
            border-left: 5px solid #667eea;
        }
        .password-box {
            background: white;
            border: 1px solid #ddd;
            border-radius: 8px;
            padding: 15px;
            font-family: monospace;
            font-size: 20px;
            word-break: break-all;
            text-align: center;
            letter-spacing: 1px;
            margin-bottom: 15px;
            user-select: all;
            cursor: pointer;
        }
        .strength {
            text-align: center;
            font-size: 14px;
            margin-top: 10px;
        }
        .strength-bar {
            height: 8px;
            background: #e0e0e0;
            border-radius: 4px;
            overflow: hidden;
            margin-top: 8px;
        }
        .strength-fill {
            height: 100%;
            width: 0%;
            transition: width 0.3s;
        }
        .error {
            background: #fee2e2;
            color: #c62828;
            padding: 12px;
            border-radius: 8px;
            margin-top: 20px;
            text-align: center;
        }
        .info {
            font-size: 12px;
            color: #888;
            text-align: center;
            margin-top: 20px;
        }
        footer {
            text-align: center;
            margin-top: 25px;
            font-size: 12px;
            color: #aaa;
        }
        @media (max-width: 480px) {
            .container { padding: 20px; }
            .password-box { font-size: 16px; }
        }
    </style>
</head>
<body>
<div class="container">
    <h1>🔐 随机密码生成器</h1>
    <div class="subtitle">安全 · 便捷 · 自定义</div>

    <form method="post" action="">
        <div class="form-group">
            <label>📏 密码长度 (4-64位)</label>
            <input type="number" name="length" min="4" max="64" value="<?php echo htmlspecialchars($length); ?>" required>
        </div>

        <div class="form-group">
            <label>🔣 包含的字符类型</label>
            <div class="checkbox-group">
                <label>
                    <input type="checkbox" name="uppercase" <?php echo $use_uppercase ? 'checked' : ''; ?>> 大写字母 (A-Z)
                </label>
                <label>
                    <input type="checkbox" name="lowercase" <?php echo $use_lowercase ? 'checked' : ''; ?>> 小写字母 (a-z)
                </label>
                <label>
                    <input type="checkbox" name="numbers" <?php echo $use_numbers ? 'checked' : ''; ?>> 数字 (0-9)
                </label>
                <label>
                    <input type="checkbox" name="symbols" <?php echo $use_symbols ? 'checked' : ''; ?>> 特殊符号 (!@#$%...)
                </label>
            </div>
        </div>

        <button type="submit">✨ 生成密码 ✨</button>
    </form>

    <?php if ($error_message !== ''): ?>
        <div class="error">⚠️ <?php echo htmlspecialchars($error_message); ?></div>
    <?php endif; ?>

    <?php if ($generated_password !== '' && $error_message === ''): ?>
        <div class="result">
            <div class="password-box" onclick="copyToClipboard(this)">
                <?php echo htmlspecialchars($generated_password); ?>
                <span style="font-size:12px; display:block; margin-top:5px;">👆 点击复制</span>
            </div>
            <div class="strength">
                密码强度：
                <?php
                $strength_text = getPasswordStrength($generated_password);
                $strength_percent = 0;
                switch ($strength_text) {
                    case '非常弱': $strength_percent = 20; break;
                    case '弱': $strength_percent = 40; break;
                    case '中等': $strength_percent = 60; break;
                    case '强': $strength_percent = 80; break;
                    case '非常强': $strength_percent = 100; break;
                }
                ?>
                <strong><?php echo $strength_text; ?></strong>
                <div class="strength-bar">
                    <div class="strength-fill" style="width: <?php echo $strength_percent; ?>%; background: <?php
                        if ($strength_percent <= 40) echo '#f44336';
                        elseif ($strength_percent <= 60) echo '#ff9800';
                        elseif ($strength_percent <= 80) echo '#2196f3';
                        else echo '#4caf50';
                    ?>;"></div>
                </div>
            </div>
        </div>
    <?php endif; ?>

    <div class="info">
        💡 提示：本生成器使用 PHP 的 <strong>random_int()</strong> 函数，提供密码学安全的随机数。
    </div>
    <footer>
        PHP 密码工具 | 安全生成 · 本地处理
    </footer>
</div>

<script>
    // 复制密码到剪贴板
    function copyToClipboard(element) {
        const password = element.innerText.replace('👆 点击复制', '').trim();
        if (!password) return;

        // 使用现代 Clipboard API
        if (navigator.clipboard && navigator.clipboard.writeText) {
            navigator.clipboard.writeText(password).then(() => {
                showToast('✅ 密码已复制到剪贴板');
            }).catch(() => {
                fallbackCopy(password);
            });
        } else {
            fallbackCopy(password);
        }
    }

    function fallbackCopy(text) {
        const textarea = document.createElement('textarea');
        textarea.value = text;
        document.body.appendChild(textarea);
        textarea.select();
        document.execCommand('copy');
        document.body.removeChild(textarea);
        showToast('📋 密码已复制 (兼容模式)');
    }

    function showToast(msg) {
        // 简单的toast提示
        let toast = document.createElement('div');
        toast.innerText = msg;
        toast.style.position = 'fixed';
        toast.style.bottom = '30px';
        toast.style.left = '50%';
        toast.style.transform = 'translateX(-50%)';
        toast.style.backgroundColor = 'rgba(0,0,0,0.8)';
        toast.style.color = 'white';
        toast.style.padding = '10px 20px';
        toast.style.borderRadius = '30px';
        toast.style.fontSize = '14px';
        toast.style.zIndex = '9999';
        toast.style.whiteSpace = 'nowrap';
        document.body.appendChild(toast);
        setTimeout(() => { toast.remove(); }, 2000);
    }
</script>
</body>
</html>