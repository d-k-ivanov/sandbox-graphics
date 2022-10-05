var count = 0;
var prev = 1;
for (let i = 0; i < 256; i++)
{
    var innfloat = Math.pow(i/255, 1/2.2);
    var found = Math.round(innfloat * 255);
    console.log("For " + i + " gamma corrected: " + innfloat + ", channel " + found);
    if (found !== prev)
    {
        ++count;
    }
    prev = found;
}
console.log("Unique gamma banding levels: " + count);
