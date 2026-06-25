import React, { useState, useEffect } from 'react'

function App() {
  const [view, setView] = useState('catalog') // catalog, exercises, dictionary
  const [data, setData] = useState([])
  const [loading, setLoading] = useState(true)

  useEffect(() => {
    setLoading(true)
    let url = 'http://localhost:8000/api/cms/articles/'
    if (view === 'exercises') {
      url = 'http://localhost:8000/api/lms/exercises/'
    } else if (view === 'dictionary') {
      url = 'http://localhost:8000/api/dictionary/vocab/'
    }

    fetch(url)
      .then(res => res.json())
      .then(resData => {
        setData(resData)
        setLoading(false)
      })
      .catch(err => {
        console.error(`Error fetching ${view}:`, err)
        setLoading(false)
      })
  }, [view])

  return (
    <div className="min-h-screen bg-white text-[#000000] font-sans text-[12pt]">
      <a name="top"></a>
      <table className="w-full border-collapse border-spacing-0" cellPadding="0" cellSpacing="0">
        <tbody>
          <tr className="align-top">
            
            {/* Left Sidebar */}
            <td className="w-[146px] h-[92px] bg-[#ffcc00] p-0 align-top">
              <a href="/">
                {/* Legacy used a logo image from configuration; using a placeholder representing it */}
                <div className="w-[146px] h-[92px] flex items-center justify-center border-b border-[#000099] opacity-80 bg-white">
                  <span className="font-bold text-[#000099]">Bohemica</span>
                </div>
              </a>
              
              <div className="bg-[#ffcc00] p-[5px]">
                <table className="w-[140px] m-0 border-spacing-0 border-collapse" cellPadding="0" cellSpacing="0">
                  <tbody>
                    <tr>
                      <td className="w-[13px] py-[1px] align-top bg-[#000099] border-[1px] border-[#ffcc00]"></td>
                      <td className="w-full bg-[#ffcc00] pt-[1px] pb-[0px] pl-[2px] align-top">
                        <span className="text-[10pt] font-bold">
                          <button onClick={() => setView('catalog')} className="text-[#0000aa] hover:text-[#aa0000] hover:underline no-underline block w-full text-left">Catalog / Home</button>
                        </span>
                      </td>
                    </tr>
                    <tr><td colSpan={2} className="h-[5px]"></td></tr>
                    <tr>
                      <td className="w-[13px] py-[1px] align-top bg-[#000099] border-[1px] border-[#ffcc00]"></td>
                      <td className="w-full bg-[#ffcc00] pt-[1px] pb-[0px] pl-[2px] align-top">
                        <span className="text-[10pt] font-bold">
                          <button onClick={() => setView('exercises')} className="text-[#0000aa] hover:text-[#aa0000] hover:underline no-underline block w-full text-left">Exercises</button>
                        </span>
                      </td>
                    </tr>
                    <tr><td colSpan={2} className="h-[5px]"></td></tr>
                    <tr>
                      <td className="w-[13px] py-[1px] align-top bg-[#000099] border-[1px] border-[#ffcc00]"></td>
                      <td className="w-full bg-[#ffcc00] pt-[1px] pb-[0px] pl-[2px] align-top">
                        <span className="text-[10pt] font-bold">
                          <button onClick={() => setView('dictionary')} className="text-[#0000aa] hover:text-[#aa0000] hover:underline no-underline block w-full text-left">Dictionary</button>
                        </span>
                      </td>
                    </tr>
                  </tbody>
                </table>
              </div>
            </td>
            
            {/* Right Main Content Column */}
            <td className="bg-white px-[5px] text-left align-top" rowSpan={2}>
              <table className="w-full border-collapse border-spacing-0" cellPadding="0" cellSpacing="0">
                <tbody>
                  <tr><td className="h-[2px]"></td></tr>
                  
                  {/* Top Right Header - Server Name */}
                  <tr>
                    <td className="w-full bg-[#000099] text-white text-[16pt] font-bold py-[5px] pl-[3px]">
                      <span>Bohemica.com</span>
                    </td>
                  </tr>
                  
                  {/* Top Right Header - Section Title */}
                  <tr>
                    <td className="w-full bg-white text-[#000099] text-[12pt] font-bold pt-0 pb-0 pl-[3px]">
                      <span className="capitalize">{view}</span>
                    </td>
                  </tr>
                  
                  {/* Top Menu Navbar */}
                  <tr>
                    <td className="w-full bg-[#ffcc00] text-black font-normal text-[8pt] pt-0 pb-0 pl-[3px]">
                      &nbsp;&nbsp;| <a href="/" className="text-[#0000aa] font-bold hover:text-[#aa0000] hover:underline no-underline">Bohemica.com</a> |
                    </td>
                  </tr>
                  
                  {/* Main Content Space */}
                  <tr>
                    <td className="pt-4 align-top">
                      {loading ? (
                        <div className="text-[10pt]">Loading...</div>
                      ) : (
                        <table className="w-full border-spacing-[0px] border-collapse" cellPadding="0">
                          <tbody>
                            {view === 'catalog' && data.map((article, idx) => (
                              <tr key={article.id} className="align-top">
                                <td className="w-full pb-4" colSpan={2}>
                                  <div className="text-justify p-[2px]">
                                    <span className="font-bold">
                                      <a href={`#article-${article.id}`} className="text-[#0000aa] hover:text-[#aa0000] hover:underline no-underline">{article.title}</a>
                                    </span>
                                    <br />
                                    <span className="text-[10pt] text-[#000000]">
                                      {article.description && <span>{article.description} -- </span>}
                                      {article.author ? article.author.name : 'Unknown'} 
                                      <small className="ml-1 text-[8pt]">(0)</small> 
                                      <small className="ml-1 text-[8pt]">[<a href={`#article-${article.id}`} className="text-[#0000aa] hover:text-[#aa0000] hover:underline no-underline font-bold">more</a>]</small>
                                    </span>
                                  </div>
                                </td>
                              </tr>
                            ))}

                            {view === 'exercises' && data.map((exercise, idx) => (
                              <tr key={exercise.id} className="align-top">
                                <td className="w-[20px] py-[2px] align-top">
                                    <div className="w-[13px] h-[13px] bg-[#000099] mt-[4px]"></div>
                                </td>
                                <td className="w-full pb-4">
                                  <div className="text-justify p-[2px]">
                                    <span className="font-bold bg-[#ffcc00] block w-full px-1">
                                      <a href={`#exercise-${exercise.id}`} className="text-[#000000] hover:text-[#aa0000] hover:underline no-underline">{exercise.name}</a>
                                    </span>
                                    <span className="text-[10pt] text-[#000000] block mt-1">
                                      {exercise.description}
                                      <br />
                                      <small className="text-[8pt]">{exercise.task}</small>
                                    </span>
                                  </div>
                                </td>
                              </tr>
                            ))}

                            {view === 'dictionary' && data.map((word, idx) => (
                              <tr key={word.id} className="align-top">
                                <td className="w-full pb-2" colSpan={2}>
                                  <div className="text-justify p-[2px]">
                                    <span className="font-bold text-[#0000aa]">
                                      {word.czech}
                                    </span>
                                    <span className="mx-2">-</span>
                                    <span className="text-[#000000]">
                                      {word.english}
                                    </span>
                                    {word.note && <span className="ml-2 italic text-[10pt] text-gray-600">({word.note})</span>}
                                  </div>
                                </td>
                              </tr>
                            ))}

                            {data.length === 0 && (
                              <tr>
                                <td className="p-4 italic text-[10pt]">No content found for this section.</td>
                              </tr>
                            )}
                          </tbody>
                        </table>
                      )}
                      
                      <br /><br />
                      
                      {/* Footer Tables inside Main Column */}
                      <table className="w-full border-spacing-0 border-collapse" cellPadding="0" cellSpacing="0">
                        <tbody>
                          <tr>
                            <td className="w-full bg-[#ffcc00] text-black text-right text-[9pt]" colSpan={2}>
                              <span>{new Date().toLocaleDateString('en-US', { weekday: 'long', day: 'numeric', month: 'short', year: 'numeric' })}</span>
                            </td>
                          </tr>
                          <tr>
                            <td className="bg-[#000099] text-white text-left p-[2px]">
                              <span className="text-[10pt]">
                                | <a href="#top" className="text-white font-bold hover:text-[#aaaaaa] no-underline">Top</a> |
                              </span>
                            </td>
                            <td className="bg-[#000099] text-white text-right p-[2px]">
                              <span>
                                <a href="/" className="text-white text-[10pt] font-bold hover:text-[#aaaaaa] no-underline">Bohemica.com</a>
                              </span>
                            </td>
                          </tr>
                        </tbody>
                      </table>
                      
                    </td>
                  </tr>
                  
                </tbody>
              </table>
            </td>
          </tr>
          
          {/* Filler row to support rowspan */}
          <tr>
            <td className="bg-[#ffcc00] align-top h-full min-h-[500px]"></td>
          </tr>
          
        </tbody>
      </table>
    </div>
  )
}

export default App