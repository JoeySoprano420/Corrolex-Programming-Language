function applyTheme(themeName) {
    const themeStyles = {
        'Shadow Realm': {
            backgroundColor: '#1e1e1e',
            color: '#ff5722',
            font: 'Shadow-Font'
        },
        'Ethereal Light': {
            backgroundColor: '#ffffff',
            color: '#03a9f4',
            font: 'Ethereal-Font'
        }
    };

    const selectedTheme = themeStyles[themeName];
    document.body.style.backgroundColor = selectedTheme.backgroundColor;
    document.body.style.color = selectedTheme.color;
    document.body.style.fontFamily = selectedTheme.font;
}
